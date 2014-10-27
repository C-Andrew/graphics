#include "scene.hpp"
#include <iostream>

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
  Matrix4x4 rotationMat;
  Vector4D r1,r2,r3,r4;

  float cosTheta = cos(angle * M_PI/ 180.0);
  float sinTheta = sin(angle * M_PI/ 180.0);

  switch(axis) {
    case 'x':
      r1 = Vector4D(1, 0, 0, 0);
      r2 = Vector4D(0, cosTheta, -sinTheta, 0);
      r3 = Vector4D(0, sinTheta, cosTheta, 0);
      r4 = Vector4D(0, 0, 0, 1);
      break;
  case 'y':

      r1 = Vector4D(cosTheta, 0, sinTheta, 0);
      r2 = Vector4D(0, 1, 0, 0);
      r3 = Vector4D(-sinTheta, 0, cosTheta, 0);
      r4 = Vector4D(0, 0, 0, 1);
      break;
  case 'z':
      r1 = Vector4D(cosTheta, -sinTheta, 0, 0);
      r2 = Vector4D(sinTheta, cosTheta, 0, 0);
      r3 = Vector4D(0, 0, 1, 0);
      r4 = Vector4D(0, 0, 0, 1);
      break;
  default:
      break;
  }

  rotationMat =  Matrix4x4(r1,r2,r3,r4);
  m_trans = m_trans * rotationMat;
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  Matrix4x4 scaleMat;
  scaleMat= Matrix4x4(Vector4D(amount[0], 0.0, 0.0, 0.0) ,
                      Vector4D(0.0, amount[1], 0.0, 0.0) ,
                      Vector4D(0.0, 0.0, amount[2], 0.0) ,
                      Vector4D(0.0, 0.0, 0.0, 1.0));

  m_trans = m_trans * scaleMat;
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  // Fill me in
  Matrix4x4 transMat;

  Vector4D r1(1, 0.0, 0.0, amount[0]);
  Vector4D r2(0.0, 1, 0.0, amount[1]);
  Vector4D r3(0.0, 0.0, 1, amount[2]);
  Vector4D r4(0.0, 0.0, 0.0, 1);

  transMat = Matrix4x4(r1,r2,r3,r4);
  m_trans = m_trans * transMat;

}

bool SceneNode::is_joint() const
{
  return false;
}

Intersection SceneNode::intersect(Ray r){
  // DO SOME SHIT
  r.direction = m_invtrans * r.direction;
  r.origin = m_invtrans * r.origin;

  Intersection intersection;
  for (std::list<SceneNode*>::const_iterator it=m_children.begin(); it != m_children.end(); ++it){
    SceneNode* node = (*it);
    Intersection t = node->intersect(r);
    if(t.hit){
      if(!intersection.hit || t.t < intersection.t){
        // std::cerr << "INTERSECTED" << std::endl;
        GeometryNode *geoNode = dynamic_cast<GeometryNode*>(*it);
        intersection.t = t.t;
        intersection.normal = t.normal;
        intersection.material = geoNode->m_material;
        intersection.primitive = geoNode->m_primitive;
        intersection.hit = true;
        intersection.point = t.point;
         // std::cerr << "2222NORMALS: " << intersection.normal << std::endl;
      }
    }
  }
  // untransform the intersection
  if (intersection.hit)
  {
    intersection.point = m_trans * intersection.point;
    intersection.normal = m_invtrans.transpose() * intersection.normal;
    intersection.normal.normalize();
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
  Intersection intersection;
  Intersection t = m_primitive->intersect(r);





  // untransform the intersection
  if (t.hit)
  {
    intersection.t = t.t;
    intersection.material = m_material;
    intersection.primitive = m_primitive;
    intersection.hit = true;
    intersection.point = m_trans * t.point;
    intersection.normal = m_invtrans.transpose() * t.normal;
    // std::cerr << "GEONODE NORMALS: " << intersection.normal << std::endl;
   
  }


  return intersection;
 }
