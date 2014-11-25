#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"
#include "image.hpp"
#include "primitive.hpp"


class Material {
public:
  Material(const Colour& kd, const Colour& ks, double shininess, double reflect, double refract, double glossy); 
  virtual ~Material();

  Colour getAmbient(const Colour& ambient);
  Colour getCoefficient(const Vector3D& normal, const Vector3D& light, const Vector3D& view) const;
  virtual Colour get_diffuse(Intersection i);
  Colour get_specular();
  double get_shiny();
  double get_reflect();
  double get_refract();
  double get_glossy();

private:
  Colour m_kd;
  Colour m_ks;
  double m_shininess;
  double m_reflect;
  double m_refract;
  double m_glossy;

};


class TextureMap : public Material {
 public:
  TextureMap(const std::string& filename, const Colour& ks, double shininess,
              double reflect, double refract, double glossy);
  virtual ~TextureMap();

 protected:
  Colour get_diffuse(Intersection i);

 private:
  Image m_textureMap;
};

#endif
