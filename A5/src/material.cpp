#include "material.hpp"

Material::~Material()
{
}

Material::Material(const Colour& kd, const Colour& ks, double shininess, double reflect, double refract, double glossy)
  : m_kd(kd), m_ks(ks), m_shininess(shininess), m_reflect(reflect), m_refract(refract), m_glossy(glossy)
{
}

Colour Material::get_diffuse(Intersection i){
	return m_kd;
}
Colour Material::get_specular(){
	return m_ks;
}
double Material::get_shiny(){
	return m_shininess;
}
double Material::get_reflect(){
	return m_reflect;
}
double Material::get_refract(){
	return m_refract;
}
double Material::get_glossy(){
	return m_glossy;
}

Colour Material::getCoefficient(const Vector3D& normal, const Vector3D& light, const Vector3D& view) const
{
  Vector3D r = -light + 2 * (light.dot(normal)) * normal;
  return m_kd + m_ks * ( pow(r.dot(view), m_shininess) / normal.dot(light) );
}

Colour Material::getAmbient(const Colour& ambient)
{
  return ambient * (m_kd);
}

/////////////////
// Texture Map //
/////////////////

TextureMap::~TextureMap()
{
}

TextureMap::TextureMap(const std::string& filename, const Colour& ks, double shininess,
              double reflect, double refract, double glossy)
	: Material(0.0, ks, shininess, reflect, refract, glossy),
		m_textureMap()
{
  if (!m_textureMap.loadPng(filename)) {
    std::cerr << "No png file found: " << filename << std::endl;
  }
}

Colour TextureMap::get_diffuse(Intersection i){
	Point2D p = i.primitive->get_texture(i.point);
	int x = p[0] * (double)m_textureMap.width();
	int y = p[1] * (double)m_textureMap.height();
  return Colour(m_textureMap(x, y, 0),
                m_textureMap(x, y, 1),
                m_textureMap(x, y, 2));
}