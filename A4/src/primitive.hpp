#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual Intersection intersect(Ray r) = 0;
  virtual Point3D get_pos() = 0;
};


class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual Intersection intersect(Ray r);
  virtual Point3D get_pos();

private:
  Point3D m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();
  virtual Intersection intersect(Ray r);
  virtual Point3D get_pos();

private:
  Point3D m_pos;
  double m_size;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual Intersection intersect(Ray r);
  virtual Point3D get_pos();
  NonhierSphere* m_sphere;
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual Intersection intersect(Ray r);
  virtual Point3D get_pos();
  NonhierBox* m_cube;
};


#endif
