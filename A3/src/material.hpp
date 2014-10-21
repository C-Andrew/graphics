#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"
#include <QMatrix4x4>

class Material {
public:
  virtual ~Material();
  virtual void apply_gl(QMatrix4x4 trans) const = 0;

protected:
  Material()
  {
  }
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
  virtual ~PhongMaterial();

  virtual void apply_gl(QMatrix4x4 trans) const;

private:
  Colour m_kd;
  Colour m_ks;

  double m_shininess;
};


#endif
