#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "polyroots.hpp"
#include <algorithm>

class Mesh;

class Primitive {
public:
  virtual ~Primitive();
  virtual Intersection intersect(Ray r) = 0;
};


class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual Intersection intersect(Ray r);
  Point3D m_pos;
private:

  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size);
  
  virtual ~NonhierBox();
  virtual Intersection intersect(Ray r);
  Point3D m_pos;

private:
  double m_size;
  Mesh *m_mesh;
};

class Cylinder : public Primitive {
public:
  virtual ~Cylinder();
  Cylinder(){};
  virtual Intersection intersect(Ray r);

};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  Sphere();
  virtual Intersection intersect(Ray r);
  NonhierSphere* m_sphere;
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  Cube();
  virtual Intersection intersect(Ray r);
  NonhierBox* m_cube;
};


#endif
