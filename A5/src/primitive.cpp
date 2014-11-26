#include <vector>
#include <list>
#include <utility>      // std::pair
#include <float.h>
#include "primitive.hpp"
#include "mesh.hpp"
#include <math.h>


////////////
// Utility//
////////////

bool inRange(double checkValue, double min, double max) {
    return (checkValue <= max && checkValue >= min);
}

bool epsilonEquals(double a, double b) {
    return abs(a - b) <= DBL_MIN;
}

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

Point2D Sphere::get_texture(const Point3D& p) const{
   return Point2D(-1, -1);
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

Point2D Cube::get_texture(const Point3D& p) const{
   return Point2D(-1, -1);
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

// Vector3D center = (r.origin - m_pos);

// double t1 = dir.dot(dir);
// double t2 = 2*dir.dot(center);
// double t3 = center.dot(center) - pow(m_radius,2);

// double root[2] = {0,0};

// if(quadraticRoots(t1,t2,t3, root) != 0){
//   double shortest = std::min(root[0], root[1]);
//   if(!intersection.hit   || shortest < intersection.t){
//     // std::cerr<<"HELLO" <<std::endl;
//     intersection.t = shortest;
//     intersection.point = r.origin + dir * shortest;
//     intersection.normal = intersection.point - m_pos;

//     intersection.hit = true;
//   }
// }

  double bsq = pow(r.direction.dot(r.origin - m_pos), 2);
  double ac = r.direction.length2() * ((r.origin - m_pos).length2() - pow(m_radius,2) ) ;
  double underRoot = bsq - ac;
  if(underRoot < -DBL_MIN){
    return intersection;
  }
  double negVal = -sqrt(underRoot);
  double posVal = sqrt(underRoot);

  double t0 = (-(r.direction.dot(r.origin-m_pos)) + negVal) / r.direction.length2();
  double t1 = (-(r.direction.dot(r.origin-m_pos)) + posVal) / r.direction.length2();
  double t = -1;
  if ((t0 > DBL_MIN) )            { t = t0; } 
  if ((t1 > DBL_MIN) && (t1 < t)) { t = t1; }
  if(t > DBL_MIN){
    Point3D temp = r.origin +  t * dir;
    // std::cerr << temp << std::endl;;
    intersection.point = temp;
    intersection.normal = temp - m_pos;
    intersection.t = t;
    intersection.hit = true;
  }

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

  intersection.u = -(atan2(m_pos[2] - intersection.point[2], m_pos[0] - intersection.point[0])) / 2 / M_PI;
  intersection.v = acos((intersection.point[1] - m_pos[1]) / m_radius) / M_PI;;
  if(intersection.u < 0){
    // std::cerr << "Sphere.u NEG" << std::endl;
  }
  if(intersection.v < 0){
    // std::cerr << "Sphere.v NEG" << std::endl;
  }
	return intersection;
}

Point2D NonhierSphere::get_texture(const Point3D& p) const{
  
  return Point2D(-1, -1);  
}

/*******************************************/

NonhierBox::NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  	// Need to create Mesh Object for cube
  	// const std::vector<Point3D>& verts
  	// Setup vertices
  	std::vector<Point3D> vertices;
  	// Build Front Face in COUNTERCLOCKERWISE order starting with bottom left vertex
  	// REMEMBER WE ARE LOOKING AT THE NEG-Z AXIS
    double h = m_size/(double)2.0;
  	vertices.push_back(Point3D( m_pos[0]-h,        m_pos[1]-h,        m_pos[2]+h ) ); 
  	vertices.push_back(Point3D( m_pos[0]-h,        m_pos[1]+h,        m_pos[2]+h ) );
  	vertices.push_back(Point3D( m_pos[0]+h,        m_pos[1]+h,        m_pos[2]+h ) );
  	vertices.push_back(Point3D( m_pos[0]+h,        m_pos[1]-h,        m_pos[2]+h ) );
  	
  	// Build Back Face in COUNTERCLOCKERWISE order starting with bottom left vertex
  	vertices.push_back(Point3D( m_pos[0]-h,        m_pos[1]-h,        m_pos[2]-h) );
  	vertices.push_back(Point3D( m_pos[0]+h,        m_pos[1]-h,        m_pos[2]-h) );
  	vertices.push_back(Point3D( m_pos[0]+h,        m_pos[1]+h,        m_pos[2]-h) );  
  	vertices.push_back(Point3D( m_pos[0]-h,        m_pos[1]+h,        m_pos[2]-h) ); 

    // // OLD
    // // Build Front Face in COUNTERCLOCKERWISE order starting with bottom left vertex
    // // REMEMBER WE ARE LOOKING AT THE NEG-Z AXIS
    // vertices.push_back(Point3D( m_pos[0],        m_pos[1],        m_pos[2]+m_size ) ); 
    // vertices.push_back(Point3D( m_pos[0],        m_pos[1]+m_size, m_pos[2]+m_size ) );
    // vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1]+m_size, m_pos[2]+m_size ) );
    // vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1],        m_pos[2]+m_size ) );
    
    // // Build Back Face in COUNTERCLOCKERWISE order starting with bottom left vertex
    // vertices.push_back(Point3D( m_pos[0],        m_pos[1],        m_pos[2]) );
    // vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1],        m_pos[2]) );
    // vertices.push_back(Point3D( m_pos[0]+m_size, m_pos[1]+m_size, m_pos[2]) );  
    // vertices.push_back(Point3D( m_pos[0],        m_pos[1]+m_size, m_pos[2]) ); 

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
       // std:: cerr << intersection.normal << std::endl;  
 //Use special origin for texture map
 // Point3D h = Point3D(m_size, m_size, m_size);
 double h = m_size/(double)2;
 double u, v;
 u = 0;
 v = 0;
  if (intersection.normal[2] == 1) { // Front Face
      u = (intersection.point[0] - h + m_size) / m_size;
      v = ((h - intersection.point[1])) / m_size;
  } else if (intersection.normal[2] == -1) { // Back Face
      u = ((h - intersection.point[0])) / m_size;
      v = ((h - intersection.point[1])) / m_size;
  } else if (intersection.normal[0] == 1) { // Right Face
      u = ( (h - intersection.point[2])) / m_size;
      v = ((h - intersection.point[1])) / m_size;
  } else if (intersection.normal[0] == -1) { // Left Face
      u = (intersection.point[2] - h) / m_size;
      v = ((h - intersection.point[1])) / m_size;
  } else if (intersection.normal[1] == 1) { // Top Face
      u = (intersection.point[0] - h - m_size) / m_size;
      v = (h - intersection.point[2]) / m_size;
  } else if (intersection.normal[1] == -1) { // Bottom Face
      u = (intersection.point[0] - h) / m_size;
      v = (h - intersection.point[2]) / m_size;
  }
   // if (intersection.normal[2] == 1) { // Front Face
   //      u = (intersection.point[0] - m_pos[0]) / m_size;
   //      v = ((intersection.point[1] - m_pos[1])) / m_size;
   //  } else if (intersection.normal[2] == -1) { // Back Face
   //      u = ((intersection.point[0] - m_pos[0])) / m_size;
   //      v = ((intersection.point[1] - m_pos[1])) / m_size;
   //  } else if (intersection.normal[0] == 1) { // Right Face
   //      u = ((intersection.point[2] - m_pos[2])) / m_size;
   //      v = ((intersection.point[1] - m_pos[1])) / m_size;
   //  } else if (intersection.normal[0] == -1) { // Left Face
   //      u = (intersection.point[2] - m_pos[2]) / m_size;
   //      v = ((intersection.point[1] - m_pos[1])) / m_size;
   //  } else if (intersection.normal[1] == 1) { // Top Face
   //      u = (intersection.point[0] - m_pos[0]) / m_size;
   //      v = (intersection.point[2] - m_pos[2]) / m_size;
   //  } else if (intersection.normal[1] == -1) { // Bottom Face
   //      u = (intersection.point[0] - m_pos[0]) / m_size;
   //      v = ((intersection.point[2] - m_pos[2])) / m_size;
   //  }
  // 190 325

  intersection.u = u;
  intersection.v = v;

 Vector3D norm =  intersection.point - Point3D( m_pos[0],        m_pos[1],        m_pos[2]) ;
 intersection.normal = norm;
 return intersection;
}

Point2D NonhierBox::get_texture(const Point3D& p) const{
   return Point2D(-1, -1);
}

/*********************************************/

Cylinder::~Cylinder(){

}

Intersection Cylinder::intersect(Ray ray){

    // From: http://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html#SECTION00023200000000000000
    Intersection intersection;

    if (ray.direction[2] == 0 || ray.direction[2] == 1) {
        return intersection;
    }

    // Solve Cylinder t values
    double a = (ray.direction[0] * ray.direction[0]) + (ray.direction[1] * ray.direction[1]);
    double b = (2 * ray.origin[0] * ray.direction[0]) + (2 * ray.origin[1] * ray.direction[1]);
    double c = (ray.origin[0] * ray.origin[0]) + (ray.origin[1] * ray.origin[1]) - 1;

    double roots[2];
    int quadResult = quadraticRoots(a, b, c, roots);

    // If no t values then no intersection
    if(quadResult == 0){
      return intersection;
    }

    // There is at least 1 intersection. Find minimum
    double Y_MAX = 1.0 + DBL_MIN;
    double Y_MIN = 0 - DBL_MIN;

    std::vector< std::pair<Point3D, double> > possiblePOI;
    double minRoot = DBL_MAX;
    for (int i = 0; i < quadResult; i++) {
        if (roots[i] < 0 + 0.1) continue;
        if (roots[i] > minRoot) continue;
        Point3D poi = ray.origin + (roots[i] * ray.direction);
        double hitX_2 = poi[0] * poi[0];
        double hitY_2 = poi[1] * poi[1];
        if (inRange(poi[2], Y_MIN, Y_MAX)) {
            minRoot = roots[i];
            possiblePOI.clear();
            std::pair<Point3D, double> p;
            p = std::make_pair(poi, roots[i]);
            possiblePOI.push_back(p);
        }
    }
    // At this point minimum root is found and stored in possiblePOI
    // 

    double t4 = (Y_MAX - ray.origin[2])/ray.direction[2];
    Point3D topPoint = ray.origin + (t4 * ray.direction);
    double topX_2 = topPoint[0] * topPoint[0];
    double topY_2 = topPoint[1] * topPoint[1];
    if ((t4 >= 0) && (topX_2 + topY_2 <= (1 + DBL_MIN))) {
        std::pair<Point3D, double> p;
        p = std::make_pair(topPoint, t4);
        possiblePOI.push_back(p);
    }


    double t3 = (Y_MIN - ray.origin[2])/ray.direction[2];
    Point3D bottomPoint = ray.origin + (t3 * ray.direction);
    double bottomX_2 = bottomPoint[0] * bottomPoint[0];
    double bottomY_2 = bottomPoint[1] * bottomPoint[1];
    if ((t3 >= 0) && (bottomX_2 + bottomY_2 <= (1 + DBL_MIN))) {
        std::pair<Point3D, double> p; 
        p = std::make_pair(bottomPoint, t3);
        possiblePOI.push_back(p);
    }

    Point3D closestPoint;
    double minDist = DBL_MAX;
    for (std::vector<std::pair<Point3D, double> >::iterator it = possiblePOI.begin(); it != possiblePOI.end(); it++) {
        std::pair<Point3D, double> p = *it;
        double distance = p.first.dist(ray.origin);
        if (distance < minDist) {
            minDist = distance;
            closestPoint = p.first;
            minRoot = p.second;
        }
    }

    if(minDist == DBL_MAX){
      return intersection;
    }

    intersection.point = closestPoint;
    intersection.hit = true;
    intersection.t = minRoot;
    if (intersection.point[2] <= Y_MIN ) {
        intersection.normal = Vector3D(0, 0, -1);
        intersection.u = (1.0 - intersection.point[0]) / 2;
        intersection.v = (1.0 - intersection.point[1]) / 2;
    } else if (intersection.point[2] >= Y_MAX) {
        intersection.normal = Vector3D(0, 0, 1);
        intersection.u = (1.0 - intersection.point[0]) / 2;
        intersection.v = (1.0 - intersection.point[1]) / 2;
    } else {
        Vector3D norm = intersection.point - Point3D(0,0,ray.origin[2]); 
        intersection.normal = norm;
        Point2D uv = get_texture(intersection.point);
        intersection.u = uv[0];
        intersection.v = uv[1];
        // To turn off reflective surfaces, uncomment next line
        intersection.normal = Vector3D(intersection.point[0], intersection.point[1], 0);
    }
    return intersection;
}


Point2D Cylinder::get_texture(const Point3D& p) const{
  int region = 0;
  double u, v;
  if (p[2] > -DBL_MIN && p[2] < DBL_MIN)  {
    region = 2;
  } else if (p[2] > 1.0 - DBL_MIN && p[2] < 1.0 + DBL_MIN) {
    region = 1;
  } else {
    region = 0;
  }

  if (region == 0) {
    double theta = acos(p[0]);

    if (p[1] > 0) {
      theta = -theta;
    }
  
    // Texture seemed stretched here so cut the range in half.
    double x = (theta + M_PI) / M_PI;
    if (x > 1) x = x -1;

    return Point2D(x, p[2]);
  } else if (region == 1) {
        u = (1.0 - p[0]) / 2;
        v = (1.0 - p[1]) / 2;
        return Point2D(u,v);
  } else if (region == 2) {
        u = (1.0 - p[0]) / 2;
        v = (1.0 - p[1]) / 2;
        return Point2D(u,v);
  } else {
    std::cerr << "Unknown Region" << std::endl;
  }

  return Point2D(-1, -1);
}

/*********************************************/

Cone::~Cone(){
}

Intersection Cone::intersect(Ray ray){
    Intersection intersection;

    if (ray.direction[2] == 0) {
        return intersection;
    }

    // Solve Cone t values
    double a = (ray.direction[0] * ray.direction[0]) + (ray.direction[1] * ray.direction[1]) - (ray.direction[2] * ray.direction[2]) ;
    double b = (2 * ray.origin[0] * ray.direction[0]) + (2 * ray.origin[1] * ray.direction[1]) - (2 * ray.origin[2] * ray.direction[2]);
    double c = (ray.origin[0] * ray.origin[0]) + (ray.origin[1] * ray.origin[1]) - (ray.origin[2] * ray.origin[2]) ;

    double roots[2];
    int quadResult = quadraticRoots(a, b, c, roots);

    // If no t values then no intersection
    if(quadResult == 0){
      return intersection;
    }

    // There is at least 1 intersection. Find minimum
    double Y_MAX = 1 + DBL_MIN;

    std::vector< std::pair<Point3D, double> > possiblePOI;
    double minRoot = DBL_MAX;
    for (int i = 0; i < quadResult; i++) {
        if (roots[i] < 0 + 0.1) continue;
        if (roots[i] > minRoot) continue;

        Point3D poi = ray.origin + (roots[i] * ray.direction);
        double hitX_2 = poi[0] * poi[0];
        double hitY_2 = poi[1] * poi[1];
        double cone_r_2 = (poi[2]) * (poi[2]);

        if (inRange(poi[2], 0, Y_MAX) && (hitX_2 + hitY_2 <= cone_r_2 + 0.1) ) {
            minRoot = roots[i];
            possiblePOI.clear();
            std::pair<Point3D, double> p;
            p = std::make_pair(poi, roots[i]);
            possiblePOI.push_back(p);
        }
    }

    // At this point minimum root is found and stored in possiblePOI
    double t3 = (0 - ray.origin[2])/ray.direction[2];
    Point3D bottomPoint = ray.origin + (t3 * ray.direction);
    double bottomX_2 = bottomPoint[0] * bottomPoint[0];
    double bottomY_2 = bottomPoint[1] * bottomPoint[1];
    if ((t3 > 0) && (bottomX_2 + bottomY_2 <= (DBL_MIN * DBL_MIN))) {
        std::pair<Point3D, double> p;
        p = std::make_pair(bottomPoint, t3);
        possiblePOI.push_back(p);
        std::cerr << "HIT BOTTOM" << std::endl;
    }

    double t4 = (Y_MAX - ray.origin[2])/ray.direction[2];
    Point3D topPoint = ray.origin + (t4 * ray.direction);
    double topX_2 = topPoint[0] * topPoint[0];
    double topY_2 = topPoint[1] * topPoint[1];
    if ((t4 >= 0) && (topX_2 + topY_2 <= (1 + DBL_MIN))) {
        std::pair<Point3D, double> p;
        p = std::make_pair(topPoint, t4);
        possiblePOI.push_back(p);
    }

    Point3D closestPoint;
    double minDist = DBL_MAX;
    for (std::vector<std::pair<Point3D, double> >::iterator it = possiblePOI.begin(); it != possiblePOI.end(); it++) {
        std::pair<Point3D, double> p = *it;
        double distance = p.first.dist(ray.origin);
        if (distance < minDist) {
            minDist = distance;
            closestPoint = p.first;
            minRoot = p.second;
        }
    }

    if(minDist == DBL_MAX){
      return intersection;
    }

    intersection.point = closestPoint;
    intersection.hit = true;
    intersection.t = minRoot;
  
    if (intersection.point[2] >= Y_MAX - DBL_MIN && intersection.point[2]<= Y_MAX + DBL_MIN + DBL_MIN )  {
      intersection.normal = Vector3D(intersection.point[0], intersection.point[1], ray.origin[2]); 

      intersection.u = (1.0 - intersection.point[0]) / 2;
      intersection.v = (1.0 - intersection.point[1]) / 2;
        // intersection.normal = Vector3D(0, 0, 1);
    } else if (intersection.point[2] <= DBL_MIN && intersection.point[2] >= -DBL_MIN) {
        intersection.u = 0.5;
        intersection.v = 0;
        intersection.normal = Vector3D(intersection.point[0], intersection.point[1], 0); 
        // intersection.normal = Vector3D(0, 0, -1);
    } else {

        Vector3D topToBase = intersection.point - Point3D();
        topToBase.normalize();

        Point3D centerPoint = Point3D(0, 0, intersection.point[2]);
        Vector3D centerToHit = intersection.point - centerPoint;
        Vector3D coneTangent = Vector3D(-centerToHit[1], centerToHit[0], 0);
        coneTangent.normalize();

        intersection.normal = coneTangent.cross(topToBase);
        intersection.u = (atan2(intersection.point[1], intersection.point[0]) + M_PI) / 2 / M_PI;
        intersection.v = intersection.point[2];
        // intersection.normal.normalize();

        // To turn off reflective surfaces uncomment this line
        // intersection.normal = Vector3D(intersection.point[0], intersection.point[1], ray.origin[2]; 
        // intersection.normal.normalize();
    }

    return intersection;
}

Point2D Cone::get_texture(const Point3D& p) const{
   return Point2D(-1, -1);
}

/**********************************************/