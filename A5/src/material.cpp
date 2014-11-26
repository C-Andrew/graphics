#include "material.hpp"

Material::~Material()
{
}

Material::Material(const Colour& kd, const Colour& ks, double shininess,
					double reflect, double refract, double glossy)
  : m_kd(kd), m_ks(ks), m_shininess(shininess), m_reflect(reflect),
  	m_refract(refract), m_glossy(glossy), m_hasBumps(false), m_bumpMap()
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

bool Material::is_bumpy(){
	return m_hasBumps;
}

bool Material::loadBumps(const std::string& filename){
	if (!m_bumpMap.loadPng(filename)) {
		std::cerr << "OMG My lumps are missing!: " << filename << std::endl;
	}
	m_hasBumps = true;
	return m_hasBumps;
}

Vector3D Material::bumpNormal(Intersection i){
	int x = i.u * (double)m_bumpMap.width();
	int y = i.v * (double)m_bumpMap.height();
	if (x < 0 || x > m_bumpMap.width() || y < 0 || y > m_bumpMap.height()) {
    	// std::cerr << "Texture Coords Off Map: " << i.u << " " << x << "  |  " << i.v << " " << y << std::endl;
    }
    if (x == 0 || y == 0 || x == m_bumpMap.width() - 1 || y == m_bumpMap.height() - 1){
    	return i.normal;
    }
	double x_gradient = 2 * (m_bumpMap(x - 1, y, 0) - m_bumpMap(x + 1, y, 0));
	double y_gradient = 2 * (m_bumpMap(x, y - 1, 0) - m_bumpMap(x, y + 1, 0));
	// if(x_gradient > 1  || x_gradient < -1){
	// 	std::cerr << "HERE" << std::endl;
	// 	x_gradient = 0.01;
	// }
	// if(y_gradient > 0.9 || y_gradient < -0.9){
	// 	y_gradient = 0.01;
	// }

	return Vector3D(i.normal[0] + x_gradient, i.normal[1] + y_gradient, i.normal[2]);

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
    std::cerr << "Could not load texture: " << filename << std::endl;
  }
}

Colour TextureMap::get_diffuse(Intersection i){
	double x = i.u * (double)m_textureMap.width();
	double y = i.v * (double)m_textureMap.height();

	// std::cerr << "x: " << x << "   y: " << y << std::endl;
  return Colour(m_textureMap(x, y, 0),
                m_textureMap(x, y, 1),
                m_textureMap(x, y, 2));
}

bool TextureMap::is_bumpy(){
	return m_hasBumps;
}

bool TextureMap::loadBumps(const std::string& filename){
	if (!m_bumpMap.loadPng(filename)) {
		std::cerr << "TEXTURE MAP: OMG My lumps are missing!: " << filename << std::endl;
	}
	m_hasBumps = true;
	return m_hasBumps;
}

Vector3D TextureMap::bumpNormal(Intersection i){
	int x = i.u * (double)m_bumpMap.width();
	int y = i.v * (double)m_bumpMap.height();
	if (x < 0 || x > m_bumpMap.width() || y < 0 || y > m_bumpMap.height()) {
    	// std::cerr << "Texture Coords Off Map: " << i.u << " " << x << "  |  " << i.v << " " << y << std::endl;
    }
    if (x == 0 || y == 0 || x == m_bumpMap.width() - 1 || y == m_bumpMap.height() - 1){
    	return i.normal;
    }
	double x_gradient = 2 * (m_bumpMap(x - 1, y, 0) - m_bumpMap(x + 1, y, 0));
	double y_gradient = 2 * (m_bumpMap(x, y - 1, 0) - m_bumpMap(x, y + 1, 0));
	// if(x_gradient > 1  || x_gradient < -1){
	// 	std::cerr << "HERE" << std::endl;
	// 	x_gradient = 0.01;
	// }
	// if(y_gradient > 0.9 || y_gradient < -0.9){
	// 	y_gradient = 0.01;
	// }

	return Vector3D(i.normal[0] + x_gradient, i.normal[1] + y_gradient, i.normal[2]);

}