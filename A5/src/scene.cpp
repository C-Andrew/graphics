#include "scene.hpp"
#include <iostream>
#include <assert.h>
#include <float.h>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::rotate(char axis, double angle)
{
  std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
  // Fill me in
  // m_rotate = m_rotate * rotation(angle, axis);
  //
  angle = (angle/180.0)*M_PI;
  Vector4D row1, row2, row3, row4;
  if(axis == 'x'){
    row1 = Vector4D(1, 0, 0, 0);
    row2 = Vector4D(0, cos(angle), -sin(angle), 0);
    row3 = Vector4D(0, sin(angle), cos(angle), 0);
    row4 = Vector4D(0, 0, 0, 1);
  }
  if(axis == 'y'){
    row1 = Vector4D(cos(angle), 0, sin(angle), 0);
    row2 = Vector4D(0, 1, 0, 0);
    row3 = Vector4D(-sin(angle), 0, cos(angle), 0);
    row4 = Vector4D(0, 0, 0, 1);
  }
  if(axis == 'z'){
    row1 = Vector4D(cos(angle), -sin(angle), 0, 0);
    row2 = Vector4D(sin(angle), cos(angle), 0, 0);
    row3 = Vector4D(0, 0, 1, 0);
    row4 = Vector4D(0, 0, 0, 1);
  }
  Matrix4x4 r = Matrix4x4(row1, row2, row3, row4);
  set_transform(m_trans * r);
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  // m_scale = m_scale * scaling(amount);
  //
  Vector4D row1 = Vector4D(amount[0], 0, 0, 0);
  Vector4D row2 = Vector4D(0, amount[1], 0, 0);
  Vector4D row3 = Vector4D(0, 0, amount[2], 0);
  Vector4D row4 = Vector4D(0, 0, 0, 1.0);
  Matrix4x4 s = Matrix4x4(row1, row2, row3, row4);
  set_transform(m_trans * s);
}

void SceneNode::translate(const Vector3D& amount)
{

    std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  // m_scale = m_scale * translation(amount);
  //
  Vector4D row1 = Vector4D(1, 0, 0, amount[0]);
  Vector4D row2 = Vector4D(0, 1, 0, amount[1]);
  Vector4D row3 = Vector4D(0, 0, 1, amount[2]);
  Vector4D row4 = Vector4D(0, 0, 0, 1);
  Matrix4x4 test = Matrix4x4(row1, row2, row3, row4);
  set_transform(m_trans * test);
  // m_trans = m_trans * t;
  // m_invtrans = m_trans.invert();
}

bool SceneNode::is_joint() const
{
  return false;
}

Intersection SceneNode::intersect(Ray r){
  Ray newRay(m_invtrans * r.origin, m_invtrans* r.direction);
  double smallest = DBL_MAX;
  
  Intersection intersection;
  std::list<SceneNode*>::const_iterator it;
  for (it = m_children.begin(); it != m_children.end(); ++it){
    SceneNode* node = (*it);
    Intersection t = node->intersect(newRay);
    if(t.hit && t.t < smallest){
        intersection = t;
        smallest = t.t;
    }
  }
  if(intersection.hit){
    intersection.point = m_trans * intersection.point;
    intersection.normal = (m_invtrans.transpose() * intersection.normal);
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
  Intersection intersection;
  Ray newRay(m_invtrans * r.origin, m_invtrans * r.direction);
    
  Intersection t = m_primitive->intersect(newRay);
  // if(t.hit){

  // }

  std::list<SceneNode*>::const_iterator it;
  // std::cerr << "name" << m_name <<std::endl;
  for (it = m_children.begin(); it != m_children.end(); ++it){
    SceneNode* node = (*it);
    // Intersection t = node->intersect(newRay);
  }


  if (t.hit)
  {
    intersection = t;
    intersection.material = m_material;
    intersection.primitive = m_primitive;
    intersection.point = m_trans * t.point;
    intersection.normal = m_invtrans.transpose() *  t.normal;
    // std::cerr << "GEONODE NORMALS: " << intersection.normal << std::endl;
   
  }


  return intersection;
 }
