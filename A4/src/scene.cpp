#include "scene.hpp"
#include <iostream>
#include <assert.h>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}


Matrix4x4 rotation(double angle, char axis)
{
  Vector4D row1;
  Vector4D row2;
  Vector4D row3;
  Vector4D row4;

  angle = angle / 180.0 * M_PI;

  switch (axis) {
    case 'x':
      row1 = Vector4D(1, 0, 0, 0);
      row2 = Vector4D(0, cos(angle), -sin(angle), 0);
      row3 = Vector4D(0, sin(angle), cos(angle), 0);
      row4 = Vector4D(0, 0, 0, 1);
    break;
    case 'y':
      row1 = Vector4D(cos(angle), 0, sin(angle), 0);
      row2 = Vector4D(0, 1, 0, 0);
      row3 = Vector4D(-sin(angle), 0, cos(angle), 0);
      row4 = Vector4D(0, 0, 0, 1);
    break;
    case 'z':
      row1 = Vector4D(cos(angle), -sin(angle), 0, 0);
      row2 = Vector4D(sin(angle), cos(angle), 0, 0);
      row3 = Vector4D(0, 0, 1, 0);
      row4 = Vector4D(0, 0, 0, 1);
    break;
    default:
      exit(1);
  }

  Matrix4x4 r(row1, row2, row3, row4);

  return r;
}

Matrix4x4 translation(const Vector3D& displacement)
{

  Vector4D row1(1, 0, 0, displacement[0]);
  Vector4D row2(0, 1, 0, displacement[1]);
  Vector4D row3(0, 0, 1, displacement[2]);
  Vector4D row4(0, 0, 0, 1);

  Matrix4x4 t(row1, row2, row3, row4);

  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
  Vector4D row1(scale[0], 0, 0, 0);
  Vector4D row2(0, scale[1], 0, 0);
  Vector4D row3(0, 0, scale[2], 0);
  Vector4D row4(0, 0, 0, 1);

  Matrix4x4 s(row1, row2, row3, row4);

  return s;
}


void SceneNode::rotate(char axis, double angle)
{
  std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
  // Fill me in
  Matrix4x4 t = rotation(angle, axis);
  Matrix4x4 matrix = m_trans * t;
  set_transform(matrix);
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  Matrix4x4 t = scaling(amount);
  Matrix4x4 matrix = m_trans * t;
  set_transform(matrix);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  // Fill me in
  Vector4D row1(1, 0, 0, amount[0]);
  Vector4D row2(0, 1, 0, amount[1]);
  Vector4D row3(0, 0, 1, amount[2]);
  Vector4D row4(0, 0, 0, 1);

  Matrix4x4 t(row1, row2, row3, row4);
  set_transform(m_trans * t);
      std::cerr << "TRANS: " << std::endl << m_trans << std::endl;
}

bool SceneNode::is_joint() const
{
  return false;
}

Intersection SceneNode::intersect(Ray r){
  // DO SOME SHIT
  // Matrix4x4 transMat;
  // Vector4D r1(1.0f, 0.0, 0.0, 100.0);
  // Vector4D r2(0.0, 1.0f, 0.0, 0.0);
  // Vector4D r3(0.0, 0.0, 1.0f, 800.0);
  // Vector4D r4(0.0, 0.0, 0.0, 1.0f);

  // transMat = Matrix4x4(r1,r2,r3,r4);
  // m_trans = m_trans * transMat;
  // m_invtrans = m_trans.invert();


  r.direction = m_invtrans * r.direction;
  r.origin = m_invtrans * r.origin;

  // std::cerr << "TRANS: " << m_trans << std::endl;
  Ray newRay(r.origin, r.direction);


  Intersection intersection;
  for (std::list<SceneNode*>::const_iterator it=m_children.begin(); it != m_children.end(); ++it){
    SceneNode* node = (*it);
    Intersection t = node->intersect(newRay);
    if(t.hit){
      if(!intersection.hit || t.t < intersection.t){
        // std::cerr << "INTERSECTED" << std::endl;
        intersection.t = t.t;
        intersection.point = t.point;
        intersection.normal = t.normal;
        intersection.hit = true;
        intersection.material = t.material;
        intersection.primitive = t.primitive;

        // intersection.normal.normalize();
         // std::cerr << "2222NORMALS: " << intersection.normal << std::endl;
      }
    }
  }
  // untransform the intersection
  if (intersection.hit)
  {
    intersection.point = m_trans * intersection.point;
    intersection.normal = m_invtrans.transpose() * intersection.normal;
  }


  return intersection;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}
 
 Intersection GeometryNode::intersect(Ray r){
    // transform ray.  Instead of moving the node, move the ray by the inverse transform
  r.direction = m_invtrans * r.direction;
  r.origin = m_invtrans * r.origin;

  // std::cerr << "GEONODE m_trans: " << m_trans << std::endl;
  Intersection intersection;
    Ray newRay(r.origin, r.direction);

  Intersection t = m_primitive->intersect(newRay);

  // untransform the intersection
  if (t.hit)
  {
    intersection.t = t.t;
    intersection.material = m_material;
    intersection.primitive = m_primitive;
    intersection.hit = true;
    intersection.point = m_trans * t.point;
    intersection.normal = m_invtrans.transpose() * t.normal;
    // intersection.normal.normalize();
    // std::cerr << "GEONODE NORMALS: " << intersection.normal << std::endl;
   
  }


  return intersection;
 }
