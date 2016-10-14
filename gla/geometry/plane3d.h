/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GLA_PLANE3D_HPP
#define GLA_PLANE3D_HPP

#include <glm/glm.hpp>
#include <limits>
#include "line3d.h"

namespace gla {


struct Plane3d
{
	Plane3d(float A, float B, float C, float D) : n(A,B,C), d( D ) { }

	Plane3d(const Point3d & point, const glm::vec3 & normal) : d(-glm::dot(normal,point.p) ), n(normal){}

	Plane3d(const Point3d & p1, const Point3d & p2, const Point3d & p3) : Plane3d(p1, glm::normalize( glm::cross( p2.p-p1.p, p3.p-p1.p) ) )
	{
	}

	Plane3d(const Point3d & p1, const Line3d & L) : Plane3d( p1, glm::cross( p1.p-L.p, L.v) )
	{
	}

	/**
		Gets the line of intersection between two planes.

		@param  P a valid plane

		@return the line of intersection between the two planes. The line is invalid if the planes do not intersect
	*/
	Line3d   Intersection( const Plane3d & P) const; // line of intersection between two planes

		
	/**
		Gets the point of intersection between a line and the plane

		@param  r a valid line

		@return the intersection point of the line and plane. The point is invalid if it does not intersect
	*/		
	Point3d Intersection( const Line3d   & r) const; // point of intersection between ray and plane

	
	/**
		Determines if the line intersects the plane.

		@param  r a valid line

		@return true if the line intersects the plane
	*/
	bool    Intersects(const Line3d & r ) const;

	/**
		Returns the distance between a Point and the plane

		@param  r a point in 3d space
		@return Distance to the plane. Negative if the point is below the plane
	*/
	float      Distance(const Point3d & r) const;

	/**
		Returns the displacement between a Point and the plane.
		
		@param  r a point in 3d space
		
		@return vector displacement needed to move the point to the plane
	*/
	glm::vec3  Displacement(const Point3d & r) const;


	/**
		Determines if a plane is valid and exists
		
		A plane is valid if its normal is non-zero and finite and it's point is finite.
			
		@return true if the plane exists
	*/
	explicit operator bool() const 
	{ 
		return std::isnormal( glm::dot(n,n) ) ;
	}
	
	glm::vec3 n; // normal, does not have to be a unit normal
	float     d; 
};

inline Line3d   Plane3d::Intersection( const Plane3d & P) const
{
	assert(false && "No implementation yet");
	
}

inline bool Plane3d::Intersects(const Line3d & r ) const
{
	return !(std::abs( glm::dot( r.v, n)) < std::numeric_limits<float>::epsilon());
}

inline Point3d Plane3d::Intersection( const Line3d & r) const
{
	return Point3d(( -d - glm::dot(n,r.p) ) / ( glm::dot(r.v,n )) * r.v + r.p);
}

inline glm::vec3  Plane3d::Displacement(const Point3d & r) const
{
	return  ((-glm::dot(n,r.p)-d)/glm::length(n))*n ;
}

inline float  Plane3d::Distance(const Point3d & r) const
{
	return ( -d - glm::dot(n, r.p) / glm::dot(n,n) );
}

}

#endif
