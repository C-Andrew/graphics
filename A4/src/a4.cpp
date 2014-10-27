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
  
  // For now, just make a sample image.

  // double fov_radius = fov * M_PI/180.0;
  double aspect = (double)width/(double)height; 

  //2. get side vector
  Vector3D side_vector = view.cross(up);
  Vector3D m_view = view;
  Vector3D m_up = up;

  //normalize view, side, up
  side_vector.normalize();
  m_view.normalize();
  m_up.normalize();

  // Camera view  unit vector
  // Vector3D cameraDirection = view;
  // cameraDirection.normalize();
  // // Camera Up unit vector
  // Vector3D cameraUp = up;
  // cameraUp.normalize();
  // // Camera X-Axis unit vector
  // Vector3D cameraX = cameraUp.cross(cameraDirection);
  // cameraX.normalize();
  // // Camera Y-Axis unit vector
  // Vector3D cameraY = cameraX.cross(cameraDirection); //This should be the same as camera up

  // Vector3D cameraDX = 2.0f * aspect * tan(fov / 2.0f) / (double)width * cameraX;
  // Vector3D cameraDY = 2.0f * aspect * tan(fov / 2.0f) / (double)height * cameraY;

  Image img(width, height, 3);
  std::list<SceneNode*> allNodes = getAllNodes(root);
  Colour prevColour(0,0,0);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      Vector3D rayOrigin(eye);
      // Magical Math as provided by 
      // http://graphics.ucsd.edu/courses/cse168_s06/ucsd/CSE168_raytrace.pdf
      double tangent = tan(fov*M_PI/360.0);
      Vector3D rayDirection(m_view + (x/(double)width * 2 - 1) * tangent * aspect * side_vector + 
                                     (y/(double)height * 2 - 1) * tangent * (-m_up) );
      rayDirection.normalize();
      Ray rayFromPixel(rayOrigin, rayDirection);

      // find the closest intersection
      Intersection minIntersection;
      
      // Check if the ray intersects any of the objects in the scene
      // This does not handle hierarchical models yet
      // Assumes all objects are attached to the root scenenode

      // minIntersection = root->intersect(rayFromPixel);
      for (SceneNode::ChildList::const_iterator it = root->m_children.begin(); it != root->m_children.end(); it++) {
        GeometryNode *geoNode = dynamic_cast<GeometryNode*>(*it);
        Intersection intersect = geoNode->m_primitive->intersect(rayFromPixel);

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
        if(y < height/2){
          img(x, y, 0) = (((double) y / (height/2)) * 110.0f)/255.0f + 10.0f/255.0f ;
          img(x, y, 1) = 0.0f;
          img(x, y, 2) = 205.0f/255.0f;  
        }
        else{
          img(x, y, 0) = 120.0f/255.0f -  (( ((double)y-(height/2.0f)) / (height/2.0f) * 110.0f )/255.0f);
          img(x, y, 1) = 0.0f;
          img(x, y, 2) = 205.0f/255.0f;  
        }

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

          // Computer Shadow
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
                // std::cerr << "SHADOWED" << std::endl;
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
          float rdotvp =  std::max( pow(r.dot(rayDirection), mat->get_shiny()) , 0.0);



          finalColour = finalColour + (attentuationFactor* (ndotl) * mat->get_diffuse() * light->colour);
          finalColour = finalColour + (attentuationFactor* (rdotvp) * mat->get_specular() * light->colour);
          // if(x > 4){
          //   Colour temp1(img(x-1,y,0), img(x-1,y,1), img(x-1,y,2) );
          //   Colour temp2(img(x-2,y,0), img(x-1,y,1), img(x-1,y,2) );
          //   Colour temp3(img(x-3,y,0), img(x-1,y,1), img(x-1,y,2) );
          //   Colour temp4(img(x-4,y,0), img(x-1,y,1), img(x-1,y,2) );
          //   finalColour = (temp1 + temp2 + temp3 + temp4 + finalColour) * 0.5f;
          // }
          // else{
          //   prevColour = finalColour;
          // }


        }// End light for-loop

        img(x,y,0) = finalColour.R();
        img(x,y,1) = finalColour.G();
        img(x,y,2) = finalColour.B();
        // std::cerr << "r:" << color[0] << "  g:"<<color[1] << "   b:" << color[2] << std::endl;

      }// End else clause
    } // End X-loop
    // std::cerr << "Rendered line: "<< y << std::endl;
  } // End Y-loop
  img.savePng(filename);
  
}


