#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <QMatrix4x4>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"

class SceneNode {
public:
  static double color_id;
  double m_color;
  double curAngle;
  bool m_selected;
  QMatrix4x4 initTrans;
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  virtual void walk_gl(bool picking = false) const;

  const QMatrix4x4& get_transform() const { return m_trans; }
  const QMatrix4x4& get_inverse() const { return m_invtrans; }
  
  void set_transform(QMatrix4x4 m)
  {
    m_trans = m;
    m_invtrans = m.inverted();
  }

  void set_transform(QMatrix4x4& m, QMatrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  std::string get_name(){
    return m_name;
  } 

  // Callbacks to be implemented.
  // These will be called from Lua.
  // That is, in *.lua, we will generate SceneNodes (which can be spheres etc) 
  // which will use these.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;

  struct JointRange {
    double min, init, max;
  };
  JointRange m_joint_x, m_joint_y;

protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;

  // Transformations
  QMatrix4x4 m_trans;
  QMatrix4x4 m_invtrans;


};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(bool bicking = false) const;

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);



  
protected:


};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  virtual void walk_gl(bool picking = false) const;

  const Material* get_material() const;
  Material* get_material(){
    return m_material;
  }

  void set_material(Material* material)
  {
    m_material = material;
  }

protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
