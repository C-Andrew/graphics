#include "scene.hpp"
#include <iostream>
#include "AppWindow.hpp"


double SceneNode::color_id =0;

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
  color_id++;
  m_color = color_id;
  m_selected = false;
  curAngle = 0;
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
  // Fill me in
  // iterate over all children
    for (std::list<SceneNode*>::const_iterator  it=m_children.begin(); it != m_children.end(); ++it){

        (*it)->walk_gl(picking);
    }
}

void SceneNode::rotate(char axis, double angle)
{
  // std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
  switch(axis){
    case 'x':

      double newAngle;
      newAngle = curAngle + angle;
      if(newAngle > m_joint_x.max || newAngle < m_joint_x.min){
        return;
      }
      else{
        m_trans.rotate(angle, 1, 0, 0);  
        curAngle = newAngle;      
      }
      break;
    case 'y':
      m_trans.rotate(angle, 0, 1, 0);
      break;
    case 'z':
      m_trans.rotate(angle, 0, 0, 1);
      break;
    default:
      break;
        
  }
}

void SceneNode::scale(const Vector3D& amount)
{
  // std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  m_trans.scale(amount[0], amount[1], amount[2]);

}

void SceneNode::translate(const Vector3D& amount)
{
  // std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  // Fill me in
  m_trans.translate(amount[0], amount[1], amount[2]);
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
  // Fill me in
  // std::cerr << "JOINT-NODE: walk_gl(" << m_name << ")" << std::endl;
  // AppWindow::m_viewer->drawSphere(m_trans, QColor(1.0, 0.0, 0.0), m_name); 
  // AppWindow::m_viewer->drawSphere(m_trans, QColor(0.0, 0.0, 1.0), m_color, picking, m_selected); 

  for (std::list<SceneNode*>::const_iterator it=m_children.begin(); it != m_children.end(); ++it){
        QMatrix4x4 orig = (*it)->get_transform();
        (*it)->set_transform(m_trans * orig);
        (*it)->walk_gl(picking);
        (*it)->set_transform(orig);
  }
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

void GeometryNode::walk_gl(bool picking) const
{
  // Fill me in
  // std::cerr << "GEO-NODE: walk_gl(" << m_name << ")" << std::endl;
  m_material->apply_gl(m_trans);
  m_primitive->walk_gl(picking, m_trans, m_color, m_selected);

}