#include "renderer.hpp"
#include <pthread.h>
#include <assert.h> 
#define NUM_THREADS 16
#define REFLECTION_DEPTH 2
#define GLOSSY_AMOUNT 10
#define JITTER_AMOUNT 1

///////////////////////
// Utility Functions //
///////////////////////

double absVal(double a) {
  if(a < 0.0){
    return -1.0 * a;
  }
  return a;
}

Colour averageColour(Colour* colourSample, double size){
  double red, green, blue;
  red = green = blue = 0;
  for(int i = 0; i < size; i++){
    red += colourSample[i].R();
    green += colourSample[i].G();
    blue += colourSample[i].B();
  }
  red = red/size;
  green = green/size;
  blue = blue/size;
  return Colour(red, green, blue);
}

/*******************/

Renderer::Renderer( SceneNode* root,
               const std::string& filename,
               int width, int height,
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               const Colour& ambient,
               const std::list<Light*>& lights,
               Image& img,
               bool enableSuperSample,
               bool enableMultiThreading):
  root(root), filename(filename), width(width), height(height), eye(eye),
  view(view), up(up), fov(fov), ambient(ambient), lights(lights), img(img),
  enableSuperSample(enableSuperSample), enableMultiThreading(enableMultiThreading)
{
  side_vector = (view.cross(up));
  normalized_view = view;
  normalized_up = up;
  normalized_left = (up.cross(view)); 

  side_vector.normalize();
  normalized_view.normalize();
  normalized_up.normalize();
  normalized_left.normalize();

  aspect = (double)width/(double)height; 
  larger_dimension = std::max(width, height);
  distance = ((double)larger_dimension / 2.0) / tan(M_PI * fov / 360); // Divide fov by 2 to get angle wrt to mid
  tangent = tan(fov*M_PI/360.0);

  Vector3D wVector = view;
  wVector.normalize();

  Vector3D u = up.cross(wVector);
  u.normalize();

  Vector3D v = wVector.cross(u);
  v.normalize();

  double d = view.length();
  double h = 2 * d * tan((fov * M_PI/180)/2);
  double w = width * h / height;

  double rotateArray[] = {
    u[0], v[0], wVector[0], 0,
    u[1], v[1], wVector[1], 0,
    u[2], v[2], wVector[2], 0,
    0, 0, 0, 1
  };

  double basisTranslate[] = {
    1, 0, 0, eye[0],
    0, 1, 0, eye[1],
    0, 0, 1, eye[2],
    0, 0, 0, 1
  };

  double scaleArray[] = {
    -h/height, 0, 0, 0,
    0, -w/width, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };

  double translate[] = {
    1, 0, 0, -1 * width/2,
    0, 1, 0, -1 * height/d,
    0, 0, 1, d,
    0, 0, 0, 1
  };

  Matrix4x4 t4 = Matrix4x4(basisTranslate);
  Matrix4x4 r3 = Matrix4x4(rotateArray);
  Matrix4x4 s2 = Matrix4x4(scaleArray);
  Matrix4x4 t1 = Matrix4x4(translate);

  Matrix4x4 WorldCoordMatrix = t4 * r3 * s2 * t1;


  // TODO: Default focal point to be something reasonable
  // focal_point = Point3D(0.0, 4.0, -16.0);
  focal_point = Point3D(0.0, 0.0, 0.0);
}

// Thread Task
// Each thread will handle [height/NUM_THREADS]# of rows
// With the last thread getting the remainder
// The task will take in the starting row and the lenght of the row (INCLUSIVE)

//Thread Data
struct ThreadArgs{
  Renderer* renderer;
  int rowStart;
  int rowEnd;
};

void* startRenderRow(void* data){
  struct ThreadArgs* args = (struct ThreadArgs*)data;
  int rowStart = args->rowStart;
  int rowEnd = args->rowEnd;
  args->renderer->renderRow(rowStart, rowEnd);
  return NULL;
}

void Renderer::render()
{
  // Fill in raytracing code here.
  // std::cerr << "Stub: render(" << root << ",\n     "
  //           << filename << ", " << width << ", " << height << ",\n     "
  //           << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
  //           << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";  // Add light handling
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;

  int rowsPerThread = (int)(ceil(height*1.0/NUM_THREADS));
  ThreadArgs* data = new ThreadArgs[NUM_THREADS];
  pthread_t threads[NUM_THREADS];
  int rc, i;
  for(i = 0; i < NUM_THREADS; i++){
    data[i].renderer = this;
    data[i].rowStart = i * rowsPerThread;
    if(i == NUM_THREADS - 1){
      data[i].rowEnd = height;
    }
    else{
      data[i].rowEnd = data[i].rowStart + rowsPerThread;
    }
    pthread_create(&threads[i], NULL, &startRenderRow, (void*)&data[i]);
  }

  for(i = 0; i < NUM_THREADS; i++){
    rc = pthread_join(threads[i], NULL);
    assert(0 == rc);
  }

  if(!enableSuperSample){
    img.savePng(filename);
  }
  else {
    // std::cerr << "Adaptive Anti-Aliasing" << std::endl;
    Colour c9[9];
    Image img2(width, height, 3);
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
          c9[0] = Colour(img(x,y,0), img(x,y,1), img(x,y,2));
          c9[1] = Colour(img(x-1,y-1,0), img(x-1,y-1,1), img(x-1,y-1,2));
          c9[2] = Colour(img(x,y-1,0), img(x,y-1,1), img(x,y-1,2));
          c9[3] = Colour(img(x+1,y-1,0), img(x+1,y-1,1), img(x+1,y-1,2));
          c9[4] = Colour(img(x-1,y,0), img(x-1,y,1), img(x-1,y,2));
          c9[5] = Colour(img(x+1,y,0), img(x+1,y,1), img(x+1,y,2));
          c9[6] = Colour(img(x-1,y+1,0), img(x-1,y+1,1), img(x-1,y+1,2));
          c9[7] = Colour(img(x,y+1,0), img(x,y+1,1), img(x,y+1,2));
          c9[8] = Colour(img(x+1,y+1,0), img(x+1,y+1,1), img(x+1,y+1,2));

          // c9[9] = Colour(img(x+2,y-1,0), img(x+2,y-1,1), img(x+2,y-1,2));
          // c9[10] = Colour(img(x+2,y,0), img(x+2,y,1), img(x+2,y,2));
          // c9[11] = Colour(img(x+2,y+1,0), img(x+2,y+1,1), img(x+2,y+1,2));

          // c9[12] = Colour(img(x-1,y-2,0), img(x-1,y-2,1), img(x-1,y-2,2));
          // c9[13] = Colour(img(x,y-2,0), img(x,y-2,1), img(x,y-2,2));
          // c9[14] = Colour(img(x+1,y-2,0), img(x+1,y-2,1), img(x+1,y-2,2));
          // c9[15] = Colour(img(x+2,y-2,0), img(x+2,y-2,1), img(x+2,y-2,2));

          // c9[16] = Colour(img(x-1,y+2,0), img(x-1,y+2,1), img(x-1,y+2,2));
          // c9[17] = Colour(img(x,y+2,0), img(x,y+2,1), img(x,y+2,2));
          // c9[18] = Colour(img(x+1,y+2,0), img(x+1,y+2,1), img(x+1,y+2,2));
          // c9[19] = Colour(img(x+2,y+2,0), img(x+2,y+2,1), img(x+2,y+2,2));

          // c9[20] = Colour(img(x-2,y-2,0), img(x+2,y-2,1), img(x+2,y-2,2));
          // c9[21] = Colour(img(x-2,y+2,0), img(x-1,y+2,1), img(x-1,y+2,2));
          // c9[22] = Colour(img(x-2,y+1,0), img(x,y+1,1), img(x,y+1,2));
          // c9[23] = Colour(img(x-2,y-1,0), img(x+1,y-1,1), img(x+1,y-1,2));
          // c9[24] = Colour(img(x-2,y,0), img(x+2,y,1), img(x+2,y,2));


          Colour adaptive = colourFromAdaptive(c9,x,y,REFLECTION_DEPTH-1);
          img2(x,y,0) = adaptive.R();
          img2(x,y,1) = adaptive.G();
          img2(x,y,2) = adaptive.B();
        }
        // std::cerr << "AAA: " << y << "/" << height << std::endl;
      }
      img2.savePng(filename);
  }
  std::cerr << "DONE" << std::endl;
}


// rowNumber = y
// lenght = x
void Renderer::renderRow(int rowStart, int rowEnd)
{
  // std::cerr << "Initiate Render rows: " << rowStart << " to " << rowEnd << std::endl;
  for(int currentRow = rowStart; currentRow < rowEnd; currentRow++){
    for(int i = 0; i < width; i++){
      Colour pixelColour1 = pixelColour(i, currentRow);
      Colour final = pixelColour1;
      // We no longer want to do inefficient super sampling
      // I will keep this here for time comparisons

      // BAD SUPER SAMPLING BAD!
      if(enableSuperSample){
        // Super Sampling X9
        // std::cerr << " BAD super sample" << std::endl;
        Colour pixelColour1 = pixelColour(i - 0.33, currentRow - 0.33);
        Colour pixelColour2 = pixelColour(i, currentRow - 0.33);
        Colour pixelColour3 = pixelColour(i + 0.33, currentRow - 0.33);
        Colour pixelColour4 = pixelColour(i - 0.33, currentRow);
        Colour pixelColour5 = pixelColour(i, currentRow);
        Colour pixelColour6 = pixelColour(i + 0.33, currentRow);
        Colour pixelColour7 = pixelColour(i - 0.33, currentRow + 0.33);
        Colour pixelColour8 = pixelColour(i, currentRow + 0.33);
        Colour pixelColour9 = pixelColour(i + 0.33, currentRow + 0.33);

        final = (double)1/9 * (pixelColour1 + pixelColour2 + pixelColour3 +
                             pixelColour4 + pixelColour5 + pixelColour6 +
                             pixelColour7 + pixelColour8 + pixelColour9);
      }

      img(i,currentRow,0) = final.R();
      img(i,currentRow,1) = final.G();
      img(i,currentRow,2) = final.B();

    }
    // std::cerr << "Rendered rows: " << currentRow << std::endl;

  }
  // std::cerr << "Rendered rows: " << rowStart << " to " << rowEnd << std::endl;
}

// Returns a new camera/eye point that is slightly "jittered" from the original
// This is si help with depth of field to provide a more "scattered" image
Point3D Renderer::jitterCamera() {
  double jitter_x =  (((double)rand() / (double)RAND_MAX) - 0.5);
  double jitter_y =  (((double)rand() / (double)RAND_MAX) - 0.5);
  Vector3D side = up.cross(view);
  return eye + jitter_x * side + jitter_y * up; 
  // return eye;
}


// Given a ray, and the height(specific y-coord) tell me what colour to draw.
Colour Renderer::pixelColour(double x, double y){   
  // // Calculate ray based on (x,y) 
  double yDisplacement = ((double)height / 2) - (double)y;
  double xDisplacement = ((double)width / 2) - (double)x;
  Vector3D rayOrigin(eye);

  // Magical Math as provided by 
  // http://graphics.ucsd.edu/courses/cse168_s06/ucsd/CSE168_raytrace.pdf
  Vector3D rayDirection(normalized_view + (x/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                   (y/(double)height * 2 - 1) * tangent * (-normalized_up) );
  rayDirection.normalize();
  // Point3D pt = WorldCoordMatrix * Point3D(x,y,0);
  // rayDirection = pt - eye;
  Ray ray(eye, rayDirection);

  // find the closest intersection
  Intersection minIntersection;

  // If focal point has been set, compute using lensing technique
  if(focal_point[0] != 0.0 || focal_point[1] != 0.0 || focal_point[2] != 0.0){

    Colour jitteredColours[JITTER_AMOUNT];
    Vector3D side = up.cross(view);

    for(int i = 0; i < JITTER_AMOUNT; i++){
      // double rand_x =  (((double)rand() / (double)RAND_MAX) - 0.5);
      // double rand_y =  (((double)rand() / (double)RAND_MAX) - 0.5);
      // Point3D jitteredEye = eye + jitter_x * side + jitter_y * up; 
      // Vector3D jitteredDirection(normalized_view + (x + rand_x/(double)width * 2 - 1) * tangent * aspect * side_vector + 
      //                              (y + rand_y/(double)height * 2 - 1) * tangent * (-normalized_up) );

      Point3D jitteredEye = jitterCamera();
      double t = (focal_point - ray.origin).dot(view) / ray.direction.dot(view);
      ray.origin = jitteredEye;
      Point3D fp = eye + t * ray.direction;
      ray.direction = fp - (WorldCoordMatrix * jitteredEye);

      minIntersection = root->intersect(ray);
      if(minIntersection.hit){
         ray.origin = eye;
         ray.direction = (normalized_view + (x/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                   (y/(double)height * 2 - 1) * tangent * (-normalized_up) );
         return colourFromRay(ray, minIntersection, 0, 1.0);
      }
      else{
        jitteredColours[i] = backgroundColour(x, y);
      }
    }
    return averageColour(jitteredColours, JITTER_AMOUNT);

  }
  else{
    // Check if the ray intersects any of the objects in the scene
    minIntersection = root->intersect(ray);

    // If intersection doesn't exist, paint background color
    if (!minIntersection.hit) {
      return backgroundColour(x, y);
    }
    else {
      //TODO
      //Get intersected object's refractive index and pass in
      Colour finalColour = colourFromRay(ray, minIntersection, 0, 1.0);
      return finalColour;
    }// End else clause
  }
}

Colour Renderer::backgroundColour(double x, double y){
    float red, green, blue;
    if(y < height/2){
      red = (((double) y / (height/2)) * 110.0f)/255.0f + 10.0f/255.0f ;
      green = 0.0f;
      blue = 205.0f/255.0f;  
    }
    else{
      red = 120.0f/255.0f -  (( ((double)y-(height/2.0f)) / (height/2.0f) * 110.0f )/255.0f);
      green = 0.0f;
      blue = 205.0f/255.0f;
    }
    return  Colour(0.0, 0.0, (double)y / height);
}

Colour Renderer::colourFromAdaptive(Colour colourSample[9], double x, double y, int recursionDepth){
  double errorEpsilon = 0.1;
  bool colourChange = false;
  for(int i = 0; i < 9; i++){
    double error = absVal(colourSample[i].R()) - absVal(colourSample[0].R());
    error += absVal(colourSample[i].G()) - absVal(colourSample[0].G());
    error += absVal(colourSample[i].B()) - absVal(colourSample[0].B());

    if(error > errorEpsilon){
      colourChange = true;
      break;
    }
  }
  if(!colourChange){
    return colourSample[0];
  }
  else{
    // return Colour(1.0, 0, 0);
    Colour c9[9];
    int counter = 0;
    for (int sx = -1; sx <= 1; sx++) {
        for (int sy = -1; sy <= 1; sy++) {
          double px = sx * 0.33 + x;
          double py = sy * 0.33 + y;
          c9[counter] = pixelColour(px, py);
          counter++;
      }
    }
    Colour averagedSample = averageColour(c9, counter);
    return averagedSample;
  }
  // return returnColour;
}

Colour Renderer::colourFromReflection(Ray ray, Intersection intersection, int recursionDepth, double refractiveIndex){
  if(recursionDepth < REFLECTION_DEPTH){
    Vector3D normal = intersection.normal;
    Vector3D reflection_direction = ray.direction - 2 * ( ray.direction.dot(normal) ) * normal;
    Ray reflection_ray(intersection.point , reflection_direction);

    Intersection reflection_intersection = root->intersect(reflection_ray);
    if(reflection_intersection.hit){
      return colourFromRay(reflection_ray, reflection_intersection, recursionDepth+1, refractiveIndex);
    }
  }
  return Colour(0.0);
}


Colour Renderer::colourFromGlossy(Ray ray, Intersection intersection, int recursionDepth, double refractiveIndex){
  if(recursionDepth < REFLECTION_DEPTH){
    Colour glossyColour(0.0);
    Vector3D normal = intersection.normal;
    normal.normalize();
    Vector3D reflect_dir = ray.direction - 2 * ( ray.direction.dot(normal) ) * normal;

    //Shoot 10 random rays 
    for(int i = 0; i < GLOSSY_AMOUNT; i++){

      Vector3D randRay_dir;
      Point3D randRay_origin;
      double a = rand() / (double) RAND_MAX;
      double b = rand() / (double) RAND_MAX;
      double theta = acos(pow((1 - a), intersection.material->get_glossy()));
      double phi = 2 * M_PI * b;
      double x = sin(phi) * cos(theta)/16;
      double y = sin(phi) * sin(theta)/16;
      double z = cos(phi);
      Vector3D u = reflect_dir.cross(intersection.normal);
      Vector3D v = reflect_dir.cross(u);
      randRay_dir = x  * u + y * v  + reflect_dir;
      randRay_dir.normalize();

      Ray glossy_ray(intersection.point, randRay_dir);
      Intersection glossy_intersection = root->intersect(glossy_ray);
      if(glossy_intersection.hit){
        glossyColour = glossyColour + colourFromRay(glossy_ray, glossy_intersection, REFLECTION_DEPTH, refractiveIndex);
      }
    }
    glossyColour =  (1/(double) GLOSSY_AMOUNT) * glossyColour;
    return glossyColour;
  }
  return Colour(0.0, 0.0, 0.0);
}

Colour Renderer::colourFromRefraction(Ray ray, Intersection intersection, int recursionDepth, double refractiveIndex){

  Vector3D vVec = intersection.point - eye;
  vVec.normalize();
  Vector3D normal = intersection.normal;

  Vector3D transmisiveRay;
  Point3D point;
  Intersection refractionIntersection;
  if(normal.dot(vVec) < 0){ // Going into sphere
    double nr = 1/intersection.material->get_refract();  
    double rootContent = std::sqrt(1 - nr * nr * (1 - (normal.dot(-vVec) *
                       (normal.dot(-vVec))))); 
    if(rootContent >= 0.0){
      transmisiveRay = (nr*(normal.dot(-vVec))-rootContent)*normal-(nr*-vVec); 
      point = intersection.point + 0.0009 * vVec;
      Ray refractionRay(point, transmisiveRay);
      refractionIntersection = root->intersect(refractionRay);
      if(refractionIntersection.hit){
        return colourFromRay(refractionRay, refractionIntersection, recursionDepth+1, refractiveIndex);
      }
    }
  }
  else{
    float nr = intersection.material->get_refract();
    float rootContent = std::sqrt(1 - nr * nr * (1-(-normal.dot(-ray.direction)* (-normal.dot(-ray.direction)))));
    if(rootContent >= 0.0){
      transmisiveRay = (nr * (-normal.dot(-ray.direction)) - rootContent) 
                        * -normal - ( nr * -ray.direction );    
      point = intersection.point + 0.0009 * ray.direction;
      Ray refractionRay(point, transmisiveRay);
      refractionIntersection = root->intersect(refractionRay);
      if(refractionIntersection.hit){
        return colourFromRay(refractionRay, refractionIntersection, recursionDepth+1, refractiveIndex);         
      }
    }
  }

  return Colour(0.0);
  // Material* mat = intersection.material;
  // double n1 = refractiveIndex;
  // double n2 =  mat->get_refract();
  // double refractRatio = 1/n1;
  // Vector3D viewDirection = intersection.point - eye;
  // Vector3D normal = intersection.normal;
  // if((ray.direction.dot(normal) < 0)){
  //   normal = -normal;
  // }
  // double cosInc = ray.direction.dot(intersection.normal);
  // double sinT2 = refractRatio * refractRatio  * (1 - cosInc * cosInc);
  // Vector3D transDirection = -refractRatio * ray.direction +
  //                          (refractRatio * cosInc - sqrt(1.0 - sinT2)) * normal;

  // Ray refractionRay(intersection.point, transDirection);
  // Intersection refractionIntersection = root->intersect(refractionRay);

  // if(recursionDepth < REFLECTION_DEPTH){
  //   if(refractionIntersection.hit){
  //     return colourFromRay(refractionRay, refractionIntersection, recursionDepth + 1, refractiveIndex);
  //   }
  //   else{
  //     std::cerr << "DONT1" << std::endl;
  //     return Colour(0);
  //   }
  // }
  // return Colour(0);

  // double n1 = refractiveIndex;
  // double n2 =  intersection.material->get_refract();
  // double n = n1/n2;
  // Vector3D normal = intersection.normal;
  // bool inside = (ray.direction.dot(normal) < 0);

  // if (!inside) {
  //   normal = -normal;
  // }

  // double cosI = normal.dot(ray.direction);
  // double sinT2 = n * n * (1 - cosI * cosI);

  // if (sinT2 > 1.0) {
  //     std::cerr << "sinT2" << sinT2 << std::endl;
  //     return colourFromRay(ray, intersection, REFLECTION_DEPTH, refractiveIndex);
  // } else {
  //   Vector3D refraction_direction = n * ray.direction - (n + std::sqrt(1 - sinT2)) * normal;
  //   Ray refraction_ray(intersection.point, refraction_direction);

  //   Intersection refraction_intersection = root->intersect(refraction_ray);
  //   if (refraction_intersection.hit) {
  //     return colourFromRay(refraction_ray, refraction_intersection, recursionDepth+1, n2);
  //   }
  // }
  // // std::cerr << "green"  << std::endl;
  // return Colour(1.0, 1.0, 0.0);
}

Colour Renderer::colourFromRay(Ray ray, Intersection minIntersection, int recursionDepth, double refractiveIndex){
  minIntersection.normal.normalize();
  Vector3D color;

  Material* mat = minIntersection.material;
  if(mat->is_bumpy()){
    minIntersection.normal = mat->bumpNormal(minIntersection);
  }
  Colour material_diffuse = mat->get_diffuse(minIntersection);
  Colour finalColour = ambient * material_diffuse;

  double reflectiveIndex = mat->get_reflect();
  double refract = mat->get_refract();
  double glossy = mat->get_glossy();
  Colour reflectColour = Colour(0.0);
  Colour glossyColour = Colour(0.0);
  Colour refractColour = Colour(0.0);
  if (reflectiveIndex > 0.0)   
    reflectColour = colourFromReflection(ray, minIntersection, recursionDepth, refractiveIndex);
  if( glossy > 0.0)
    glossyColour = colourFromGlossy(ray, minIntersection, recursionDepth, refractiveIndex);
  if( refract > 0.0)
    refractColour = colourFromRefraction(ray, minIntersection, recursionDepth, refractiveIndex);

  for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); it++) {
    Light * light = *it;

    Vector3D light_vector = (light->position) - (minIntersection.point);
    light_vector.normalize();

    // Compute Shadow
    Vector3D lightDir = (minIntersection.point)-(light->position);
    lightDir.normalize();
    Ray objToLight(light->position, lightDir);
    bool shadowed = false;
    double distToLight = ( minIntersection.point- light->position).length();


    Intersection lightIntersection = root->intersect(objToLight);
    if (lightIntersection.hit){
        // std::cerr << "HIT" << node->m_name << std::endl;
        if ( (lightIntersection.point - light->position).length() < distToLight - 0.001)
        {
          shadowed = true;
        }
    }
    if(shadowed){
      continue;
    }


    float distLightToObj = light_vector.length(); // distance form light souce to POI
    float attentuationFactor =  1.0 /
                              (light->falloff[0] + light->falloff[1]*distLightToObj + light->falloff[2]*distLightToObj*distLightToObj);

    light_vector.normalize(); 
    float ndotl = std::max( minIntersection.normal.dot(light_vector) , 0.0);


    Vector3D r =  light_vector - (2.0f * (light_vector.dot(minIntersection.normal)) * minIntersection.normal);
    float rdotvp =  std::max( pow(r.dot(ray.direction), mat->get_shiny()) , 0.0);

    Colour diffuse = ( (ndotl) * material_diffuse * attentuationFactor * light->colour);
    Colour specular = ( (rdotvp) * mat->get_specular() * attentuationFactor * light->colour);

    finalColour = finalColour + diffuse + specular;
  }
  if(refract > 0){
    return (1.0 - 0.4) * finalColour + 0.4 * refractColour;
  }
  return (1.0 - reflectiveIndex - glossy) * finalColour + reflectiveIndex * reflectColour + glossy * glossyColour + refractColour;
}


// std::list<SceneNode*> getAllNodes(SceneNode* root){
//   std::list<SceneNode*> retVal;
//   std::list<SceneNode*> nodes;
//   nodes.push_back(root);
//   int i = 0;
//   while(!nodes.empty()){
//     SceneNode* currentNode = nodes.front();
//     nodes.pop_front();
//     for (std::list<SceneNode*>::iterator it=currentNode->m_children.begin(); it != currentNode->m_children.end(); it++){
//         nodes.push_back(*it);
//         i++;
//     }
//     retVal.push_back(currentNode);
//   }
//   std::cerr << "Num of Nodes:" << i << std::endl;
//    return retVal;
// }
