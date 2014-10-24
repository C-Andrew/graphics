#include "primitive.hpp"

Primitive::~Primitive()
{
}


/////////////////////////////
// Hierarchical Primitives //
/////////////////////////////
Sphere::~Sphere()
{
}

Intersection Sphere::intersect(Ray r){
  return m_sphere->intersect(r);
  //return 0.0f;
}

Point3D Sphere::get_pos(){
	return m_sphere->get_pos();
}

/**********************************************/

Cube::~Cube()
{
}

Intersection Cube::intersect(Ray r){
 return m_cube->intersect(r);
}

Point3D Cube::get_pos(){
	return  m_cube->get_pos();
}


////////////////////////////////
// Nonhierarchical primitives //
////////////////////////////////

NonhierSphere::~NonhierSphere()
{
}

Intersection NonhierSphere::intersect(Ray r){
	Intersection intersection;	

	Vector3D origin = r.origin;
	Vector3D dir = r.direction;
	Vector3D normal;	

	// x = Xe + t(Xp -Xe) ..y..z
	// x^2 + y^2 + z^2 = 1
	Vector3D vPos = Vector3D(m_pos);
	Vector3D dist = vPos - origin; 
	double B = dir.dot( dist);
	double D = B*B - dist.dot( dist) + m_radius * m_radius; // A.dot(A) = ||A||^2

	if (D < 0.0f) 
	    return intersection; 
	double t0 = B - sqrt(D); 
	double t1 = B + sqrt(D);
	double t = -1;  

	if ((t0 > 0.1f) )            { t = t0; } 
	if ((t1 > 0.1f) && (t1 < t)) { t = t1; }
	if(t > 0){
		Vector3D temp = origin + dir * t;
		normal = m_pos - temp;
		normal.normalize();
		intersection.normal = normal;
		intersection.t = t;
		
	}
	return intersection;
}

Point3D NonhierSphere::get_pos(){
	return m_pos;
}

/**********************************************/

NonhierBox::~NonhierBox()
{
}

Point3D NonhierBox::get_pos(){
	return m_pos;
}

Intersection NonhierBox::intersect(Ray r){
 Intersection intersection;
 return intersection;
}
