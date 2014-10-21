#include "material.hpp"
#include "AppWindow.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl(QMatrix4x4 trans) const
{
  // Perform OpenGL calls necessary to set up this material.
	AppWindow::m_viewer->setupMaterial(m_kd, m_ks, m_shininess, trans);
}
