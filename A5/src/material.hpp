#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"

class Material {
public:
  Material(const Colour& kd, const Colour& ks, double shininess, double reflect, double refract); 
  virtual ~Material();

  Colour getAmbient(const Colour& ambient);
  Colour getCoefficient(const Vector3D& normal, const Vector3D& light, const Vector3D& view) const;
  Colour get_diffuse();
  Colour get_specular();
  double get_shiny();
  double get_reflect();
  double get_refract();

private:
  Colour m_kd;
  Colour m_ks;
  double m_shininess;
  double m_reflect;
  double m_refract;

};

#endif
