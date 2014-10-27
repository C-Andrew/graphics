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
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif

// #ifndef CS488_MESH_HPP
// #define CS488_MESH_HPP
// #include <vector>
// #include <iosfwd>
// #include "primitive.hpp"
// #include "algebra.hpp"
// // A polygonal mesh.
// class Mesh : public Primitive {
// public:
//   Mesh(const std::vector<Point3D>& verts,
//        const std::vector< std::vector<int> >& faces);
//   ~Mesh();
//   typedef std::vector<int> Face;
//   virtual Intersection intersect(Ray ray);
//   virtual Point3D get_pos();

//   // NonhierBox* boundingSphere;
//   std::vector<Vector3D> faceNormals;
//   std::vector<std::vector<Vector3D> > edgeNormals;
// private:
//   std::vector<Point3D> m_verts;
//   std::vector<Face> m_faces;
//   std::vector<double> uMinV;
//   std::vector<double> uMaxV;
//   std::vector<double> vMinV;
//   std::vector<double> vMaxV;
//   friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
// };
// #endif