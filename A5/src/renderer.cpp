#include "renderer.hpp"
#include <pthread.h>
#include <assert.h> 
#define NUM_THREADS     3

float rand_green = rand();
float rand_blue = (rand() % 205) / 255.0f;

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
  largerDimension = std::max(width, height);
  distance = ((double)largerDimension / 2.0) / tan(M_PI * fov / 360); // Divide fov by 2 to get angle wrt to mid
  tangent = tan(fov*M_PI/360.0);

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
  std::cerr << "Stub: render(" << root << ",\n     "
            << filename << ", " << width << ", " << height << ",\n     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
            << ambient << ",\n     {";

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
    if(i == NUM_THREADS-1){
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

  // for (int y = 0; y != height;) {
  //   data[y].renderer = this;
  //   data[y].rowStart = y;

  //   if(height - y > height/NUM_THREADS){
  //     data[y].rowEnd = y+(height/NUM_THREADS);
  //     y += height/NUM_THREADS;    
  //   }
  //   else if(height - y < height/NUM_THREADS){
  //     data[y].rowEnd = height;
  //     y = height; 
  //   }
  // } // End Y-loop
  img.savePng(filename);
  
}

// rowNumber = y
// lenght = x
void Renderer::renderRow(int rowStart, int rowEnd)
{
  for(int currentRow = rowStart; currentRow < rowEnd; currentRow++){
    for(int i = 0; i < width; i++){
      double yDisplacement = ((double)height / 2) - (double)currentRow;
      double xDisplacement = ((double)width / 2) - (double)i;
      Vector3D rayOrigin(eye);

      // Magical Math as provided by 
      // http://graphics.ucsd.edu/courses/cse168_s06/ucsd/CSE168_raytrace.pdf

      Vector3D rayDirection1(distance * normalized_view + yDisplacement * normalized_up + xDisplacement * normalized_left);
      rayDirection1.normalize();
      Ray rayFromPixel1(eye, rayDirection1);
      Colour pixelColour1 = colourFromRay(rayFromPixel1, currentRow);
      Colour final = pixelColour1;
      if(enableSuperSample){
        // Super Sampling X9
        // std::cerr << "super sample" << std::endl;
        Vector3D rayDirection1(normalized_view + ((i-0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       ((currentRow-0.33f)/(double)height * 2 - 1) * tangent * (-normalized_up) );
        Vector3D rayDirection2(normalized_view + ((i)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       ((currentRow-0.33f)/(double)height * 2 - 1) * tangent * (-normalized_up) );
        Vector3D rayDirection3(normalized_view + ((i+0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       ((currentRow-0.33f)/(double)height * 2 - 1) * tangent * (-normalized_up) );
        
        Vector3D rayDirection4(normalized_view + ((i-0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       ((currentRow)/(double)height * 2 - 1) * tangent * (-normalized_up) );
        Vector3D rayDirection5(normalized_view + (i/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       (currentRow/(double)height * 2 - 1) * tangent * (-normalized_up) );
        Vector3D rayDirection6(normalized_view + ((i+0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       (currentRow/(double)height * 2 - 1) * tangent * (-normalized_up) );
        
        Vector3D rayDirection7(normalized_view + ((i-0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       ((currentRow+0.33f)/(double)height * 2 - 1) * tangent * (-normalized_up) );
        Vector3D rayDirection8(normalized_view + ((i)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       ((currentRow+0.33f)/(double)height * 2 - 1) * tangent * (-normalized_up) );
        Vector3D rayDirection9(normalized_view + ((i+0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                       ((currentRow+0.33f)/(double)height * 2 - 1) * tangent * (-normalized_up) );
        
        rayDirection1.normalize();
        rayDirection2.normalize();
        rayDirection3.normalize();
        rayDirection4.normalize();
        rayDirection5.normalize();
        rayDirection6.normalize();
        rayDirection7.normalize();
        rayDirection8.normalize();
        rayDirection9.normalize();  

        Ray rayFromPixel1(eye, rayDirection1);
        Ray rayFromPixel2(eye, rayDirection2);
        Ray rayFromPixel3(eye, rayDirection3);
        Ray rayFromPixel4(eye, rayDirection4);
        Ray rayFromPixel5(eye, rayDirection5);
        Ray rayFromPixel6(eye, rayDirection6);
        Ray rayFromPixel7(eye, rayDirection7);
        Ray rayFromPixel8(eye, rayDirection8);
        Ray rayFromPixel9(eye, rayDirection9);

        Colour pixelColour1 = colourFromRay(rayFromPixel1, currentRow);
        Colour pixelColour2 = colourFromRay(rayFromPixel2, currentRow);
        Colour pixelColour3 = colourFromRay(rayFromPixel3, currentRow);
        Colour pixelColour4 = colourFromRay(rayFromPixel4, currentRow);
        Colour pixelColour5 = colourFromRay(rayFromPixel5, currentRow);
        Colour pixelColour6 = colourFromRay(rayFromPixel6, currentRow);
        Colour pixelColour7 = colourFromRay(rayFromPixel7, currentRow);
        Colour pixelColour8 = colourFromRay(rayFromPixel8, currentRow);
        Colour pixelColour9 = colourFromRay(rayFromPixel9, currentRow);

        final = (double)1/9 * (pixelColour1 + pixelColour2 + pixelColour3 +
                             pixelColour4 + pixelColour5 + pixelColour6 +
                             pixelColour7 + pixelColour8 + pixelColour9);
      }
      img(i,currentRow,0) = final.R();
      img(i,currentRow,1) = final.G();
      img(i,currentRow,2) = final.B();

    }
    std::cerr << "Rendered rows: " << currentRow << std::endl;

  }
  std::cerr << "Rendered rows: " << rowStart << " to " << rowEnd << std::endl;
}


// Given a ray, and the height(specific y-coord) tell me what colour to draw.
Colour Renderer::colourFromRay(Ray ray, int y)
{
      // find the closest intersection
      Intersection minIntersection;

      // Check if the ray intersects any of the objects in the scene
      minIntersection = root->intersect(ray);

      // If intersection doesn't exist, paint background color
      if (!minIntersection.hit) {
        float red, green, blue;
        if(y < height/2){
          red = (((double) y / (height/2)) * 110.0f)/255.0f + 10.0f/255.0f ;
          green = 0.0f;
          blue = rand_blue;  
        }
        else{
          red = 120.0f/255.0f -  (( ((double)y-(height/2.0f)) / (height/2.0f) * 110.0f )/255.0f);
          green = 0.0f;
          blue = rand_blue;
        }
        return Colour(red, green, blue);
      }

      else {
        minIntersection.normal.normalize();
        Vector3D color;

        Material* mat = minIntersection.material;
        Colour finalColour = ambient * mat->get_diffuse();

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

          Colour diffuse = ( (ndotl) * mat->get_diffuse() * light->colour);
          Colour specular = ( (rdotvp) * mat->get_specular() * light->colour);

          finalColour = finalColour  + (diffuse) + ( specular);

        }// End light for-loop
        return finalColour;
      }// End else clause

}

std::list<SceneNode*> getAllNodes(SceneNode* root){
  std::list<SceneNode*> retVal;
  std::list<SceneNode*> nodes;
  nodes.push_back(root);
  int i = 0;
  while(!nodes.empty()){
    SceneNode* currentNode = nodes.front();
    nodes.pop_front();
    for (std::list<SceneNode*>::iterator it=currentNode->m_children.begin(); it != currentNode->m_children.end(); it++){
        nodes.push_back(*it);
        i++;
    }
    retVal.push_back(currentNode);
  }
  std::cerr << "Num of Nodes:" << i << std::endl;
   return retVal;
}
