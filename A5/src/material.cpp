#include "material.hpp"

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess, double reflect, double refract)
  : m_kd(kd), m_ks(ks), m_shininess(shininess), m_reflect(reflect), m_refract(refract)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.
}

Colour PhongMaterial::get_diffuse(){
	return m_kd;
}
Colour PhongMaterial::get_specular(){
	return m_ks;
}
double PhongMaterial::get_shiny(){
	return m_shininess;
}
double PhongMaterial::get_reflect(){
	return m_reflect;
}
double PhongMaterial::get_refract(){
	return m_refract;
}

