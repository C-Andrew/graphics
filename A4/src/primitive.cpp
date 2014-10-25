#include <vector>
#include "primitive.hpp"
#include "mesh.hpp"

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

NonhierBox::NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  	// Need to create Mesh Object for cube
  	// const std::vector<Point3D>& verts
  	// Setup vertices
  	std::vector<Point3D> vertices;
  	// Build Front Face in clockwise order starting with bottom left vertex
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1],        m_pos[2]) );
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1]+m_size, m_pos[2]) );
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1]+m_size, m_pos[2]) );
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1],        m_pos[2]) );
  	
  	// Build Back Face in clockwise order starting with bottom left vertex
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1],        m_pos[2]+m_size) ); 
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1]+m_size,  m_pos[2]+m_size) ); 
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1]+m_size, m_pos[2]+m_size) ); 
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1],        m_pos[2]+m_size) ); 

  	//const std::vector< std::vector<int> >& faces)
  	std::vector< std::vector<int> > faces;
  	// Front face
  	std::vector<int> front_face;
  	front_face.push_back(0); 
  	front_face.push_back(1); 
  	front_face.push_back(2); 
  	front_face.push_back(3);

  	std::vector<int> back_face;
  	back_face.push_back(4); 
  	back_face.push_back(5); 
  	back_face.push_back(6); 
  	back_face.push_back(7);

  	std::vector<int> left_face;
  	left_face.push_back(0); 
  	left_face.push_back(1); 
  	left_face.push_back(5); 
  	left_face.push_back(4);

  	std::vector<int> right_face;
  	right_face.push_back(3); 
  	right_face.push_back(2); 
  	right_face.push_back(6); 
  	right_face.push_back(7);

  	std::vector<int> top_face;
  	top_face.push_back(1); 
  	top_face.push_back(5); 
  	top_face.push_back(6); 
  	top_face.push_back(2);

  	std::vector<int> bottom_face;
  	bottom_face.push_back(0); 
  	bottom_face.push_back(4); 
  	bottom_face.push_back(7); 
  	bottom_face.push_back(3);

	faces.push_back(front_face);
	faces.push_back(top_face);
	faces.push_back(back_face);
	faces.push_back(bottom_face);
	faces.push_back(left_face);
	faces.push_back(right_face);

	m_mesh = new Mesh(vertices, faces);

  }

NonhierBox::~NonhierBox()
{
	delete m_mesh;
}

Point3D NonhierBox::get_pos(){
	return m_pos;
}

Intersection NonhierBox::intersect(Ray r){
 Intersection intersection = m_mesh->intersect(r);
 return intersection;
}
