#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"

class Material {
public:
  virtual ~Material();
  virtual void apply_gl() const = 0;
  virtual Colour get_diffuse() =0;
  virtual Colour get_specular() =0;
  virtual double get_shiny() =0;
  virtual double get_reflect() =0;
  virtual double get_refract() =0;


protected:
  Material()
  {
  }
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess, double reflect, double refract);
  virtual ~PhongMaterial();
  virtual void apply_gl() const;

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
