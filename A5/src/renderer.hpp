#ifndef CS488_RENDERER_HPP
#define CS488_RENDERER_HPP

#include <math.h>
#include <string>
#include <algorithm>    // std::max
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "image.hpp"


class Renderer{
public:
     SceneNode* root;
     // Where to output the image
     const std::string& filename;

     // Image size
     int width; int height;

     // Viewing parameters
     const Point3D& eye; const Vector3D& view;
     const Vector3D& up; double fov;

     // Lighting parameters
     const Colour& ambient;
     const std::list<Light*>& lights;

     // Additional Command Line Options
     bool enableSuperSample; 
     bool enableMultiThreading;

     // Additional member variables needed
     Vector3D side_vector;
     Vector3D normalized_view;
     Vector3D normalized_up;
     Vector3D normalized_left;
     Image& img;

     double aspect;
     int largerDimension;
     double distance;
     double tangent;

     Renderer( SceneNode* root,
               const std::string& filename,
               int width, int height,
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               const Colour& ambient,
               const std::list<Light*>& lights,
               Image& img,
               bool enableSuperSample,
               bool enableMultiThreading);
     void render();
     Colour pixelColour(double x, double y);
     Colour colourFromReflection(Ray ray, Intersection intersection, int recursionDepth, int refractiveIndex);
     Colour colourFromRefraction(Ray ray, Intersection intersection, int recursionDepth, int refractiveIndex);
     Colour colourFromGlossy(Ray ray, Intersection intersection, int recursionDepth, int refractiveIndex);
     Colour colourFromAdaptive(Colour colourSample[9], double x, double y, int recursionDepth);
     Colour colourFromRay(Ray ray, Intersection minIntersection, int recursionDepth, int refractiveIndex);
     void renderRow(int rowNumber, int length);
};

#endif
