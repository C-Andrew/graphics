#include "primitive.hpp"
#include "AppWindow.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(bool picking, QMatrix4x4 m, double color_id, bool isSelected) const
{
  // Fill me in
	AppWindow::m_viewer->drawSphere(m, QColor(0.0, 0.0, 1.0), color_id, picking, isSelected);	
}
