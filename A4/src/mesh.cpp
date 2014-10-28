#include "mesh.hpp"
#include <iostream>
#include <algorithm>    // std::max

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
  float maxX, maxY, maxZ;
  float minX, minY, minZ;
  maxX= maxY= maxZ= minX= minY= minZ = 0;


  std::vector<Point3D>::const_iterator I;
  for (I = m_verts.begin(); I != m_verts.end(); ++I) {
    Point3D pt = (*I);
    if(pt[0] > maxX ){ maxX =  pt[0]; }
    if(pt[1] > maxX ){ maxY =  pt[1]; }
    if(pt[2] > maxX ){ maxZ =  pt[2]; }

    if(pt[0] < minX ){ minX =  pt[0]; }
    if(pt[1] < minY ){ minY =  pt[1]; }
    if(pt[2] < minZ ){ minZ =  pt[2]; }
  }
  far = Point3D(maxX, maxY, maxZ);
  near = Point3D(minX, minY, minZ);
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}

bool Mesh::intersectBoundingBox(Ray r){
  // X CHECKS
  float tmin = (near[0] - r.origin[0]) / r.direction[0];
  float tmax = (far[0] - r.origin[0]) / r.direction[0];

  if(tmin > tmax){
    float temp = tmin;
    tmin = tmax;
    tmax = temp;
  }

  // Y CHECKS
  float tymin = (near[1] - r.origin[1]) / r.direction[1];
  float tymax = (far[1] - r.origin[1]) / r.direction[1];
  if (tymin > tymax){
    float temp = tymin;
    tymin = tymax;
    tymax = temp;
  }
  if ((tmin > tymax) || (tymin > tmax)) {return false;}
  if (tymin > tmin)
      tmin = tymin;
  if (tymax < tmax)
      tmax = tymax;

  // Z CHECKS
  float tzmin = (near[2] - r.origin[2]) / r.direction[2];
  float tzmax = (far[2] - r.origin[2]) / r.direction[2];
  if (tzmin > tzmax){
    float temp = tzmin;
    tzmin = tzmax;
    tzmax = temp;
  }
  if ((tmin > tymax) || (tymin > tmax)){ return false; }
  return true;

}



Intersection Mesh::intersect(Ray r){

  Intersection intersection;
  // Check if the ray intersects your magical box
  // If it does, do real intersection
  if(!intersectBoundingBox(r)){
    return intersection;
  }

  //For evert Face, check intersection
  std::vector<Face>::const_iterator it;
  for (it = m_faces.begin(); it != m_faces.end(); it++)
  {
    std::vector<int> face = (*it);
    // Create a vector<Point3D> to represent a face of the mesh
    std::vector<Point3D> faceAsVertices;
    for(unsigned int i = 0; i < face.size(); i++){
      faceAsVertices.push_back( m_verts[face[i]] );
    }


    Intersection curIntersect = intersectFace(r, faceAsVertices);

    if(curIntersect.hit){
      if(!intersection.hit || curIntersect.t < intersection.t){
        intersection = curIntersect;
      }
    }

  }
  return intersection;
}

Intersection Mesh::intersectFace(Ray r, std::vector<Point3D> face){
      // std::cerr << "ENTER" << std::endl;
  Intersection intersection;

  Vector3D face_normal = Vector3D(face[2]-face[1]).cross( Vector3D(face[0]-face[1]) );
  face_normal.normalize();

  // float denominator = dotProduct(vectorSub(Ray.R2, Poly.P[0]), face_normal)
  float denominator = face_normal.dot(r.direction);
  float numerator = face_normal.dot(face[0] - Point3D(r.origin[0], r.origin[1], r.origin[2]));
  if( denominator == 0.00f ){ 
    return intersection;
  }

  float t_val = numerator / denominator;
  if(t_val <= 0.0f){ return intersection; }

  Point3D pt = Point3D(r.origin[0], r.origin[1], r.origin[2]) + t_val*r.direction;
  
  bool inside = false;
  for (unsigned int i = 0; i < face.size(); i++)
  {
    Vector3D edgeOfFace = ((face[(i+1)%face.size()]) - face[i]);
    Vector3D edgeToVertex = pt - face[i];
    float d = (edgeOfFace.cross(edgeToVertex)).dot(face_normal);
    if( d < 0.001f) {
      inside = false;
      break;
    }
    inside = true;
        
  }

  if(inside){
    intersection.point = pt;
    intersection.t = t_val;
    intersection.normal = face_normal;
    intersection.hit = true;
  }
  return intersection;

}

Point3D Mesh::get_pos(){
  Point3D p(0.0f, 0.0f, 0.0f);
  return p;
}

// 