#include "primitive.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

float Sphere::intersect(Ray r){
	return 0.0f;
}

Cube::~Cube()
{
}

float Cube::intersect(Ray r){
	return 0.0f;
}

NonhierSphere::~NonhierSphere()
{
}

float NonhierSphere::intersect(Ray r){
	
	Vector3D origin = r.origin;
	Vector3D dir = r.direction;

	Vector3D vPos = Vector3D(m_pos);
	Vector3D dist = vPos - origin; 
	double B = dir.dot( dist);
	double D = B*B - dist.dot( dist) + m_radius * m_radius; 

	if (D < 0.0f) 
	    return -1; 
	double t0 = B - sqrt(D); 
	double t1 = B + sqrt(D);
	double retValue = -1;  

	if ((t0 > 0.1f) ) 
	{	
		retValue = t0;
	} 
	if ((t1 > 0.1f) && (t1 < retValue)) 
	{
	    retValue = t1;
	}
	return retValue;

}


NonhierBox::~NonhierBox()
{
}

float NonhierBox::intersect(Ray r){
	return 0.0f;
}