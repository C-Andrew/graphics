#ifndef CS488_LIGHT_HPP
#define CS488_LIGHT_HPP

#include "algebra.hpp"
#include <iosfwd>

// Represents a simple point light.
struct Light {
  Light();
  
  Colour colour;
  Point3D position;
  double falloff[3];
};

std::ostream& operator<<(std::ostream& out, const Light& l);

#endif

// double attenFactor = 1/(light->falloff[0] + (light->falloff[1] * r) + light->falloff[2] * r * r);
//     lightColor = attenFactor * lightColor;

//     diffuse = (kd * lightColor * ndotL);
//     specular = (material->m_ks * lightColor * rDotVp);

//     colour = diffuse + speccular + ambient

