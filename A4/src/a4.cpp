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

  double fov_radius = fov*M_PI/180.0;
  //2. get side vector
  Vector3D side_vector = view.cross(up);
  Vector3D m_view = view;
  Vector3D m_up = up;
  //normalize view, side, up
  side_vector.normalize();
  m_view.normalize();
  m_up.normalize();


  Image img(width, height, 3);
  std::list<SceneNode*> allNodes = getAllNodes(root);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < height; x++) {
      // For every pixel (x,y)
      // Cast a ray

      Vector3D rayOrigin(x - width/2.f,height/2.f - y , 500.f);
      Vector3D rayDirection(0.f, 0.f, -1.f);
      Ray rayFromPixel(rayOrigin, rayDirection);
      // find the closest intersection

      double bestIntersection = -1;

      for (SceneNode::ChildList::const_iterator it = root->m_children.begin(); it != root->m_children.end(); it++) {
        GeometryNode *geoNode = dynamic_cast<GeometryNode*>(*it);
        assert(geoNode);
        double t = geoNode->m_primitive->intersect(rayFromPixel);

        if (t > 0) {
          if (bestIntersection < 0) bestIntersection = t;
          else if (t < bestIntersection) bestIntersection = t;
        }
      }
      // given an intersection, compute if a light is visible
      // compute the color based on the light


      // Set the color


      if (bestIntersection < 0) {

        // Red: increasing from top to bottom
        img(x, y, 0) = (double)y / height;
        // Green: increasing from left to right
        img(x, y, 1) = (double)x / width;
        // Blue: in lower-left and upper-right corners
        img(x, y, 2) = ((y < height/2 && x < height/2)
                        || (y >= height/2 && x >= height/2)) ? 1.0 : 0.0;
      }
      else {
        img(x,y,0) = 0.0;
        img(x,y,1) = 1.0;
        img(x,y,2) = 0.0;

      }
    }
  }
  img.savePng(filename);
  
}


