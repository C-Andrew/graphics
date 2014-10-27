#include "a4.hpp"
#include "image.hpp"
#include <assert.h> 

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

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               )
{
  // Fill in raytracing code here.

  std::cerr << "Stub: a4_render(" << root << ",\n     "
            << filename << ", " << width << ", " << height << ",\n     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
            << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";  // Add light handling
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;
  
  double aspect = (double)width/(double)height; 

  Vector3D side_vector = view.cross(up);
  Vector3D m_view = view;
  Vector3D m_up = up;

  //normalize view, side, up
  side_vector.normalize();
  m_view.normalize();
  m_up.normalize();

  Image img(width, height, 3);
  std::list<SceneNode*> allNodes = getAllNodes(root);
  double tangent = tan(fov*M_PI/360.0);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      Vector3D rayOrigin(eye);
      // Magical Math as provided by 
      // http://graphics.ucsd.edu/courses/cse168_s06/ucsd/CSE168_raytrace.pdf

      // Super Sampling X9
      Vector3D rayDirection1(m_view + ((x-0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     ((y-0.33f)/(double)height * 2 - 1) * tangent * (-m_up) );
      Vector3D rayDirection2(m_view + ((x)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     ((y-0.33f)/(double)height * 2 - 1) * tangent * (-m_up) );
      Vector3D rayDirection3(m_view + ((x+0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     ((y-0.33f)/(double)height * 2 - 1) * tangent * (-m_up) );
      
      Vector3D rayDirection4(m_view + ((x-0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     ((y)/(double)height * 2 - 1) * tangent * (-m_up) );
      Vector3D rayDirection5(m_view + (x/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     (y/(double)height * 2 - 1) * tangent * (-m_up) );
      Vector3D rayDirection6(m_view + ((x+0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     (y/(double)height * 2 - 1) * tangent * (-m_up) );
      
      Vector3D rayDirection7(m_view + ((x-0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     ((y+0.33f)/(double)height * 2 - 1) * tangent * (-m_up) );
      Vector3D rayDirection8(m_view + ((x)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     ((y+0.33f)/(double)height * 2 - 1) * tangent * (-m_up) );
      Vector3D rayDirection9(m_view + ((x+0.33f)/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     ((y+0.33f)/(double)height * 2 - 1) * tangent * (-m_up) );

      rayDirection1.normalize();
      rayDirection2.normalize();
      rayDirection3.normalize();
      rayDirection4.normalize();
      rayDirection5.normalize();
      rayDirection6.normalize();
      rayDirection7.normalize();
      rayDirection8.normalize();
      rayDirection9.normalize();

      Ray rayFromPixel1(rayOrigin, rayDirection1);
      Ray rayFromPixel2(rayOrigin, rayDirection2);
      Ray rayFromPixel3(rayOrigin, rayDirection3);
      Ray rayFromPixel4(rayOrigin, rayDirection4);
      Ray rayFromPixel5(rayOrigin, rayDirection5);
      Ray rayFromPixel6(rayOrigin, rayDirection6);
      Ray rayFromPixel7(rayOrigin, rayDirection7);
      Ray rayFromPixel8(rayOrigin, rayDirection8);
      Ray rayFromPixel9(rayOrigin, rayDirection9);

      Colour pixelColour1 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel1, y);
      Colour pixelColour2 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel2, y);
      Colour pixelColour3 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel3, y);
      Colour pixelColour4 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel4, y);
      Colour pixelColour5 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel5, y);
      Colour pixelColour6 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel6, y);
      Colour pixelColour7 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel7, y);
      Colour pixelColour8 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel8, y);
      Colour pixelColour9 = colourFromRay(root, height, ambient,
                                  lights, rayFromPixel9, y);

      Colour final = (float)1/9 * (pixelColour1 + pixelColour2 + pixelColour3 +
                                   pixelColour4 + pixelColour5 + pixelColour6 +
                                   pixelColour7 + pixelColour8 + pixelColour9) ;
        img(x,y,0) = final.R();
        img(x,y,1) = final.G();
        img(x,y,2) = final.B();
    } // End X-loop
    if(y%10 == 0){
      std::cerr << "Rendered line: "<< y << std::endl;      
    }

  } // End Y-loop
  img.savePng(filename);
  
}

// Given a ray, tell me what colour to draw.
Colour colourFromRay( 
               // What to render
               SceneNode* root,
                int height,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights,
               Ray ray, int y)
{


      // find the closest intersection
      Intersection minIntersection;
      
      // Check if the ray intersects any of the objects in the scene
      // This does not handle hierarchical models yet
      // Assumes all objects are attached to the root scenenode

      // minIntersection = root->intersect(rayFromPixel);
      for (SceneNode::ChildList::const_iterator it = root->m_children.begin(); it != root->m_children.end(); it++) {
        GeometryNode *geoNode = dynamic_cast<GeometryNode*>(*it);
        Intersection intersect = geoNode->m_primitive->intersect(ray);

        if (intersect.hit) {
          if(!minIntersection.hit|| intersect.t < minIntersection.t){
            minIntersection.t = intersect.t;
            minIntersection.material = geoNode->m_material;
            minIntersection.primitive = geoNode->m_primitive;
            minIntersection.normal = intersect.normal;
            minIntersection.hit = true;
            minIntersection.point = intersect.point;
          } 
        }
      }

      // If intersection doesn't exist, paint background color
      if (!minIntersection.hit) {
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
        return Colour(red, green, blue);
      }

      else {
        Vector3D color;

        Material* mat = minIntersection.material;
        Primitive* prim = minIntersection.primitive;

        Colour finalColour = ambient * mat->get_diffuse();

        for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); it++) {
          Light * light = *it;

          Vector3D light_vector = (light->position) - (minIntersection.point);
          light_vector.normalize();

          // Compute Shadow
          Vector3D lightDir = (minIntersection.point)-(light->position);
          lightDir.normalize();
          Ray objToLight(Vector3D(light->position), lightDir);
          bool shadowed = false;
          double distToLight = ( minIntersection.point- light->position).length();

          for (SceneNode::ChildList::const_iterator it = root->m_children.begin(); it != root->m_children.end(); it++)
          { 
            GeometryNode *node = dynamic_cast<GeometryNode*>(*it);
            Intersection lightIntersection = node->m_primitive->intersect(objToLight);

            if (lightIntersection.hit){
              // std::cerr << "HIT" << node->m_name << std::endl;
              if ( (lightIntersection.point - light->position).length() < distToLight - 0.001)
              {
                shadowed = true;
                break;
              }
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


          Vector3D r = light_vector -  (2.0f * (light_vector.dot(minIntersection.normal)) * minIntersection.normal);
          float rdotvp =  std::max( pow(r.dot(ray.direction), mat->get_shiny()) , 0.0);



          finalColour = finalColour + (attentuationFactor* (ndotl) * mat->get_diffuse() * light->colour);
          finalColour = finalColour + (attentuationFactor* (rdotvp) * mat->get_specular() * light->colour);

        }// End light for-loop
        return finalColour;
      }// End else clause

}