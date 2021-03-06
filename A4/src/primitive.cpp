#include <vector>
#include "primitive.hpp"
#include "mesh.hpp"
#include <math.h>

Primitive::~Primitive()
{
}


/////////////////////////////
// Hierarchical Primitives //
/////////////////////////////
Sphere::Sphere(){
  m_sphere = new NonhierSphere(Point3D(0.0, 0.0, 0.0), 1.0);
}

Sphere::~Sphere()
{
  delete m_sphere;
}

Intersection Sphere::intersect(Ray r){
  return m_sphere->intersect(r);
  //return 0.0f;
}

/**********************************************/

Cube::~Cube()
{
  delete m_cube;
}

Cube::Cube(){
  m_cube = new NonhierBox(Point3D(0.0,0.0,0.0), 1.0);
}

Intersection Cube::intersect(Ray r){
 return m_cube->intersect(r);
}


////////////////////////////////
// Nonhierarchical primitives //
////////////////////////////////

NonhierSphere::~NonhierSphere()
{
}



Intersection NonhierSphere::intersect(Ray r){
	Intersection intersection;	

	Point3D origin = r.origin;
	Vector3D dir = r.direction;
	Vector3D normal;	

Vector3D center = (r.origin - m_pos);

double t1 = dir.dot(dir);
double t2 = 2*dir.dot(center);
double t3 = center.dot(center) - pow(m_radius,2);

double root[2] = {0,0};

if(quadraticRoots(t1,t2,t3, root) != 0){
  double shortest = std::min(root[0], root[1]);
  if(!intersection.hit   || shortest < intersection.t){
    // std::cerr<<"HELLO" <<std::endl;
    intersection.t = shortest;
    intersection.point = r.origin + dir * shortest;
    intersection.normal = intersection.point - m_pos;

    intersection.hit = true;
  }
}

  // double bsq = pow(r.direction.dot(r.origin - m_pos), 2);
  // double ac = r.direction.length2() * ((r.origin - m_pos).length2() - pow(m_radius,2) ) ;
  // double underRoot = bsq - ac;
  // if(underRoot < 0.1f){
  //   return intersection;
  // }
  // double negVal = -sqrt(underRoot);
  // double posVal = sqrt(underRoot);

  // double t0 = (-(r.direction.dot(r.origin-m_pos)) + negVal) / r.direction.length2();
  // double t1 = (-(r.direction.dot(r.origin-m_pos)) + posVal) / r.direction.length2();
  // double t = -1;
  // if ((t0 > 0.1f) )            { t = t0; } 
  // if ((t1 > 0.1f) && (t1 < t)) { t = t1; }
  // if(t > 0){
  //   Point3D temp = r.origin + dir * t;
  //   intersection.point = temp;
  //   intersection.normal = temp - m_pos;
  //   intersection.t = t;
  //   intersection.hit = true;
  // }

	// x = Xe + t(Xp -Xe) ..y..z
	// x^2 + y^2 + z^2 = 1
	// Vector3D dist = m_pos - r.origin; 
	// double B = dir.dot( dist);
	// double D = B*B - dist.dot( dist) + m_radius * m_radius; // A.dot(A) = ||A||^2

	// if (D < 0.0f) 
	//     return intersection; 
	// double t0 = B - sqrt(D); 
	// double t1 = B + sqrt(D);
	// double t = -1;  

	// if ((t0 > 0.1f) )            { t = t0; } 
	// if ((t1 > 0.1f) && (t1 < t)) { t = t1; }
	// if(t > 0){
	// 	Point3D temp = origin + dir * t;
	// 	intersection.point = temp;
	// 	intersection.normal = temp - m_pos;
	// 	intersection.t = t;
	// 	intersection.hit = true;
	// }
	return intersection;
}

/**********************************************/

NonhierBox::NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  	// Need to create Mesh Object for cube
  	// const std::vector<Point3D>& verts
  	// Setup vertices
  	std::vector<Point3D> vertices;
  	// Build Front Face in COUNTERCLOCKERWISE order starting with bottom left vertex
  	// REMEMBER WE ARE LOOKING AT THE NEG-Z AXIS
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1],        m_pos[2]+m_size ) ); 
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1]+m_size, m_pos[2]+m_size ) );
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1]+m_size, m_pos[2]+m_size ) );
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1],        m_pos[2]+m_size ) );
  	
  	// Build Back Face in COUNTERCLOCKERWISE order starting with bottom left vertex
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1],        m_pos[2]) );
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1],        m_pos[2]) );
  	vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1]+m_size, m_pos[2]) );  
  	vertices.push_back(Point3D( m_pos[0],        m_pos[1]+m_size, m_pos[2]) ); 

  	//const std::vector< std::vector<int> >& faces)
  	std::vector< std::vector<int> > faces;
  	// Push them in counterclockwise order startingwith loweest value
  	std::vector<int> front_face;
    front_face.push_back(3);
    front_face.push_back(2); 
    front_face.push_back(1); 
  	front_face.push_back(0); 

  	std::vector<int> back_face;
    back_face.push_back(7);
    back_face.push_back(6); 
    back_face.push_back(5); 
    back_face.push_back(4); 

  	std::vector<int> left_face;
  	left_face.push_back(4);
    left_face.push_back(5); 
    left_face.push_back(3); 
    left_face.push_back(0); 

  	std::vector<int> right_face;
  	right_face.push_back(2);
    right_face.push_back(6); 
    right_face.push_back(7); 
    right_face.push_back(1); 

  	std::vector<int> top_face;
  	top_face.push_back(3);
    top_face.push_back(5); 
    top_face.push_back(6); 
    top_face.push_back(2); 

  	std::vector<int> bottom_face;
  	bottom_face.push_back(1);
    bottom_face.push_back(7); 
    bottom_face.push_back(4); 
    bottom_face.push_back(0); 

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

Intersection NonhierBox::intersect(Ray r){
 Intersection intersection = m_mesh->intersect(r);
 return intersection;
}
