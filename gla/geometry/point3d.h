#ifndef GLA_POINT3D_HPP
#define GLA_POINT3D_HPP

#include <glm/glm.hpp>
#include <cmath>

namespace gla {
// a simple wrapper for a point class.
struct Point3d
{
	Point3d(float x=0.0f, float y=0.0f, float z=0.0f) : p(x,y,z){}
	
	Point3d(const glm::vec3 & P) : p(P){}

	Point3d(const Point3d & P)   : p( P.p ) {}

	Point3d & operator==(const glm::vec3 & P)
	{
		p = P;
		return *this;
	}

	explicit operator bool() const 
	{ 
		return (std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z)); 
	}

	glm::vec3 p;
};


// difference of two points results in a vector
inline glm::vec3 operator-(const Point3d & left, const Point3d & right )
{
	return left.p-right.p;
}


// point + vector = point
inline Point3d operator+(const Point3d & left, const glm::vec3 & right )
{
	return Point3d(left.p+right);
}
// point - vector = point
inline Point3d operator-(const Point3d & left, const glm::vec3 & right )
{
	return Point3d(left.p - right);
}



inline Point3d& operator+=(Point3d &left, const glm::vec3 & right )
{
	left.p+=right;
	return left;
}

inline Point3d& operator-=(Point3d &left, const glm::vec3 & right )
{
	left.p-=right;
	return left;
}

}

#endif
