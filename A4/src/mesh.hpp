#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);

  typedef std::vector<int> Face;
  virtual Intersection intersect(Ray r);
  Intersection intersectFace(Ray r, std::vector<Point3D> face);
  virtual Point3D get_pos();
  //bounding box 
  Point3D far;
  Point3D near;
  float size;
  float sizeX;
  float sizeY;
  float sizeZ;
  std::vector<Point3D> box_vertices;
  std::vector< std::vector<Point3D> > box_faces;
  bool intersectBoundingBox(Ray r);

private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
