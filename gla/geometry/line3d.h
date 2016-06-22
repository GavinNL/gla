#ifndef GLA_LINE3D_HPP
#define GLA_LINE3D_HPP

#include "point3d.h"

namespace gla {

struct Line3d
{
	Line3d() : p(0.0f), v(1.0f,0.0f,0.0f) {}

	Line3d( const Point3d & point, const glm::vec3 & dir) : p(point.p), v(dir) { }
	
	Line3d( const Point3d & p1, const Point3d & p2) : p(p1.p), v( p2.p-p1.p ) {}

	/**
		Used to translate the point along the direction vector	
		
		@param  t a scalar parameter to translate the vector by.

		@return A point on the line.
	*/		
	Point3d operator()(float t) const
	{
		return Point3d(p+t*v);
	}

	
	/**
		Gets the shortest distance from the point to the line. 
		
		@param  p a valid point

		@return shortest distance to the line from the point
	*/	
	float     Distance(const Point3d & p) const;

	/**
		Gets the displacement of the point to the line

		Gets the translation vector required to move the point to the line
		
		@param  p a valid plane

		@return A vector of the displacement
	*/
	glm::vec3 Displacement(const Point3d & p) const;

	
	/**
		Gets the shortest distance between two lines

		@param  L a valid line

		@return the shortest distance between the lines
	*/
	float     Distance(    const Line3d & L) const;

	/**
		Gets the line segment between two lines.

		If L1 and L2 are two lines and L3 is the shortest distance between the two lines
		
		then L3(0.0f) is a point on L1
		an   L3(1.0f) is a point on L2
		
		@param  L a valid line

		@return The line segment connecting the two lines at its shortest location
	*/
	Line3d    Displacement(const Line3d & L) const;	

	
	explicit operator bool() const 
	{ 
		return std::isnormal( glm::dot(v,v) );
	}
	
	glm::vec3 p;
	glm::vec3 v;
};

inline float Line3d::Distance(const Line3d & L) const
{
	return Displacement(L).v.length();
}

inline Line3d Line3d::Displacement(const Line3d & P) const
{
	glm::vec3 n  = glm::cross(v  , P.v);
	
	glm::vec3 n1 = glm::cross(v  , n); 
	glm::vec3 n2 = glm::cross(P.v, n);

	auto t = glm::dot(P.p-p, n1)	/ glm::dot(P.v,n1);
	auto s = glm::dot(p-P.p, n2)	/ glm::dot(  v,n2);
	
	return Line3d(  Point3d(p+s*v) ,   P.p+t*P.v - p-s*v );
}



inline float     Line3d::Distance(const Point3d & p) const
{
	return Displacement(p).length();
}

inline glm::vec3 Line3d::Displacement(const Point3d & P) const
{
	glm::vec3 ap = p - P.p;
	
	return ap - v * (glm::dot(ap,v) / glm::dot(v,v));
}

}

#endif
