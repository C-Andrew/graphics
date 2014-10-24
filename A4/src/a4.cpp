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

  double fov_radius = fov * M_PI/180.0;
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
  Vector3D cameraDirection = view;
  cameraDirection.normalize();
  // Camera Up unit vector
  Vector3D cameraUp = up;
  cameraUp.normalize();
  // Camera X-Axis unit vector
  Vector3D cameraX = cameraDirection.cross(cameraUp);
  cameraX.normalize();
  // Camera Y-Axis unit vector
  Vector3D cameraY = cameraDirection.cross(cameraX); //This should be the same as camera up

  Vector3D cameraDX = 2.0f * aspect * tan(fov / 2.0f) / (double)width * cameraX;
  Vector3D cameraDY = 2.0f * tan(fov / 2.0f) / (double)height * cameraY;


  Image img(width, height, 3);
  std::list<SceneNode*> allNodes = getAllNodes(root);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < height; x++) {
      // For every pixel (x,y)
      // Cast a ray
      Vector3D rayOrigin(x - width/2.0f, height/2.0f - y, 500.0f); // TODO NEEDS TO BE EYE
      Vector3D rayDirection(0.0f, 0.0f, -1.0f);

      // Vector3D rayOrigin(eye);
      // Vector3D rayDirection(cameraDirection - 0.5f * (2*y+1-height) * cameraDY + 0.5f * (2*x+1-width) * cameraDX);
      // rayDirection.normalize();
      Ray rayFromPixel(rayOrigin, rayDirection);

      // find the closest intersection
      Intersection minIntersection;
      
      // Check if the ray intersects any of the objects in the scene
      // This does not handle hierarchical models yet
      // Assumes all objects are attached to the root scenenode
      for (SceneNode::ChildList::const_iterator it = root->m_children.begin(); it != root->m_children.end(); it++) {
        GeometryNode *geoNode = dynamic_cast<GeometryNode*>(*it); // ASSUMING SceneNode only has spheres for now
        Intersection intersect = geoNode->m_primitive->intersect(rayFromPixel);

        if (intersect.t > 0) {
      	  if(minIntersection.t < 0 || intersect.t < minIntersection.t){
        		minIntersection.t = intersect.t;
            minIntersection.node = geoNode;
            minIntersection.normal = intersect.normal;
      	  }	
        }
      }

      // If intersection doesn't exist, paint background color
      if (minIntersection.t < 0 || minIntersection.normal.dot(rayDirection) <= 0 ) {

        // Red: increasing from top to bottom
        img(x, y, 0) = (double)y / height;
        // Green: increasing from left to right
        img(x, y, 1) = (double)x / width;
        // Blue: in lower-left and upper-right corners
        img(x, y, 2) = ((y < height/2 && x < height/2)
                        || (y >= height/2 && x >= height/2)) ? 1.0 : 0.0;
      }
      else {
        /*img(x,y,0) = 0.0;
        img(x,y,1) = 1.0;
        img(x,y,2) = 0.0;*/
        std::cerr << "T: " <<  minIntersection.t << std::endl;
        Vector3D color;

        Material* mat = minIntersection.node->m_material;
        Primitive* prim = minIntersection.node->m_primitive;

        Colour finalColour = ambient;

        for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); it++) {
          Light * light = *it;

          Vector3D light_vector = (minIntersection.point) - (light->position);
          float distLightToObj = light_vector.length(); // distance form light souce to POI
          float attentuationFactor =  1.0 /
                                    (light->falloff[0] + light->falloff[1]*distLightToObj + light->falloff[2]*distLightToObj*distLightToObj);

          light_vector.normalize(); 
          float ndotl = std::max( minIntersection.normal.dot(light_vector) , 0.0);


          Vector3D r = (-1.0f * light_vector) + (2.0f * (ndotl) * minIntersection.normal);
          float rdotvp =  std::max( pow(r.dot(rayDirection), mat->get_shiny()) , 0.0);


          finalColour = finalColour + attentuationFactor * (ndotl) * mat->get_diffuse() * light->colour;
          finalColour = finalColour + attentuationFactor * (rdotvp) * mat->get_specular() * light->colour;

        }

        img(x,y,0) = finalColour.R();
        img(x,y,1) = finalColour.G();
        img(x,y,2) = finalColour.B();
        // std::cerr << "r:" << color[0] << "  g:"<<color[1] << "   b:" << color[2] << std::endl;

      }
    }
  }
  img.savePng(filename);
  
}


