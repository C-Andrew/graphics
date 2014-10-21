#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include <QMatrix4x4>

class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl(bool picking, QMatrix4x4 m, double color_id,bool isSelected) const = 0;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual void walk_gl(bool picking, QMatrix4x4 m, double color_id, bool isSelected) const;
};

#endif
