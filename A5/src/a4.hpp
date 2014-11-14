#ifndef CS488_A4_HPP
#define CS488_A4_HPP

#include <math.h>
#include <string>
#include <algorithm>    // std::max
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"

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
               const std::list<Light*>& lights,
               bool enableSuperSample, 
               bool enableMultiThreading
               );

Colour colourFromRay( 
               // What to render
               SceneNode* root,
               int height,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights,
               Ray ray, int y);

#endif
