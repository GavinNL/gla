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
