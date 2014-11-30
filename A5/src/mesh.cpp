#include "mesh.hpp"
#include <iostream>
#include <float.h>
#include <algorithm>    // std::max

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces),
     m_boundingSphere(Point3D(0.0, 0.0, 0.0), 0.0)
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
  float max = std::max(maxX, maxY);
  max = std::max(max, maxZ);
  float min = std::min(minX, minY);
  min = std::min(min, minZ);

  size = max - min;
  sizeX = maxX - minX;
  sizeY = maxY - minY;
  sizeZ = maxZ - minZ;


    // Build Front Face in COUNTERCLOCKERWISE order starting with bottom left vertex
    // REMEMBER WE ARE LOOKING AT THE NEG-Z AXIS
    box_vertices.push_back(Point3D( near[0],        near[1],        near[2]+sizeZ ) ); 
    box_vertices.push_back(Point3D( near[0],        near[1]+sizeY, near[2]+sizeZ ) );
    box_vertices.push_back(Point3D( near[0]+sizeX, near[1]+sizeY, near[2]+sizeZ ) );
    box_vertices.push_back(Point3D( near[0]+sizeX, near[1],        near[2]+sizeZ ) );
    
    // Build Back Face in COUNTERCLOCKERWISE order starting with bottom left vertex
    box_vertices.push_back(Point3D( near[0],        near[1],        near[2]) );
    box_vertices.push_back(Point3D( near[0]+sizeX, near[1],        near[2]) );
    box_vertices.push_back(Point3D( near[0]+sizeX, near[1]+sizeY, near[2]) );  
    box_vertices.push_back(Point3D( near[0],        near[1]+sizeY, near[2]) ); 

    // Push them in counterclockwise order startingwith loweest value
    std::vector<Point3D> front_face;
    front_face.push_back(box_vertices[3]);
    front_face.push_back(box_vertices[2]); 
    front_face.push_back(box_vertices[1]); 
    front_face.push_back(box_vertices[0]); 

    std::vector<Point3D> back_face;
    back_face.push_back(box_vertices[7]);
    back_face.push_back(box_vertices[6]); 
    back_face.push_back(box_vertices[5]); 
    back_face.push_back(box_vertices[4]); 

    std::vector<Point3D> left_face;
    left_face.push_back(box_vertices[4]);
    left_face.push_back(box_vertices[5]); 
    left_face.push_back(box_vertices[3]); 
    left_face.push_back(box_vertices[0]); 

    std::vector<Point3D> right_face;
    right_face.push_back(box_vertices[2]);
    right_face.push_back(box_vertices[6]); 
    right_face.push_back(box_vertices[7]); 
    right_face.push_back(box_vertices[1]); 

    std::vector<Point3D> top_face;
    top_face.push_back(box_vertices[3]);
    top_face.push_back(box_vertices[5]); 
    top_face.push_back(box_vertices[6]); 
    top_face.push_back(box_vertices[2]); 

    std::vector<Point3D> bottom_face;
    bottom_face.push_back(box_vertices[1]);
    bottom_face.push_back(box_vertices[7]); 
    bottom_face.push_back(box_vertices[4]); 
    bottom_face.push_back(box_vertices[0]);

    box_faces.push_back(front_face);
    box_faces.push_back(top_face);
    box_faces.push_back(back_face);
    box_faces.push_back(bottom_face);
    box_faces.push_back(left_face);
    box_faces.push_back(right_face); 

  // bounding sphere

  double maxDist = 0.0;
  Point3D point1, point2;
  std::vector<Point3D>::const_iterator p1;
  std::vector<Point3D>::const_iterator p2;
  for (p1 = m_verts.begin(); p1 != m_verts.end(); p1++) {
    for (p2 = p1 + 1; p2 != m_verts.end(); p2++) {
      double dist = (*p1 - *p2).length();
      if (dist > maxDist) {
        maxDist = dist;
        point1 = *p1;
        point2 = *p2;
      }
    }
  }

  double radius = maxDist / 2.0;
  std::cerr << radius << std::endl;
  Point3D center = (1.0/2.0) * (point1 + point2);
  m_boundingSphere = NonhierSphere(center, radius);

}

Point2D Mesh::get_texture(const Point3D& p) const{
   return Point2D(-1, -1);
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


//For evert Face, check intersection
  // Intersection intersection;
  // std::vector< std::vector<Point3D> >::const_iterator it;
  // for (it = box_faces.begin(); it != box_faces.end(); it++)
  // {
  //   std::vector<Point3D> faceAsVertices = (*it);

  //   Intersection curIntersect = intersectFace(r, faceAsVertices);

  //   if(curIntersect.hit){
  //     if(!intersection.hit || curIntersect.t < intersection.t){
  //       intersection = curIntersect;
  //     }
  //   }

  // }
  // return intersection;

}



Intersection Mesh::intersect(Ray r){

  Intersection intersection;
  // Check if the ray intersects your magical box
  // If it does, do real intersection
  // Intersection boundingBoxIntersection = intersectBoundingBox(r);
  Intersection boundSphere = m_boundingSphere.intersect(r);
  if(!boundSphere.hit){
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

  Vector3D face_normal = Vector3D(face[1]-face[0]).cross( Vector3D(face[2]-face[1]) );
  face_normal.normalize();
  // float denominator = dotProduct(vectorSub(Ray.R2, Poly.P[0]), face_normal)
  double denominator = face_normal.dot(r.direction);
  double numerator = face_normal.dot(face[0] - r.origin);
  if( denominator >= 0.1 ){ return intersection; }

  double t_val = numerator / denominator;
  if(t_val <= 0.1){ return intersection; }

  Point3D pt = r.origin + t_val*r.direction;
  
  bool inside = false;
  for (unsigned int i = 0; i < face.size(); i++)
  {
    Vector3D edgeOfFace = ((face[(i+1)%face.size()]) - face[i]);
    Vector3D edgeToVertex = pt - face[i];
    double d = (edgeOfFace.cross(edgeToVertex)).dot(face_normal);
    if(d >= 0){
      inside = true;      
    }
    else{
      inside = false;
      break;
    }

        
  }

  if(inside){
    intersection.point = pt;
    intersection.t = t_val;
    intersection.normal = face_normal;
    intersection.hit = true;
  }
  return intersection;

}

// 