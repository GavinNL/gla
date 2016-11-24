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

#ifndef GLA_BOUNDING_RECT
#define GLA_BOUNDING_RECT

#include <glm/glm.hpp>

namespace gla
{

template<typename T=float>
struct BoundingRect
{

    //using vec_type = glm::detail::tvec2<T, glm::defaultp>;
    using vec_type = glm::tvec2<T, glm::defaultp>;

    vec_type min;
    vec_type max;
	
    //glm::vec3 x;

    BoundingRect( const vec_type & pMin) : min(pMin), max(pMin)
	{
	}
	
    BoundingRect( const vec_type & pMin, const vec_type & pMax) : min(pMin), max(pMax)
	{
		
	}

    BoundingRect()
    {

    }

    vec_type Size() const
    {
        return max - min;
    }

    vec_type Centre() const
    {
        return 0.5f*(min + max);
    }

    BoundingRect<T>& operator+=(const vec_type & x )
    {
        min += x;
        max += x;
        return *this;
    }
    BoundingRect<T>& operator-=(const vec_type & x )
    {
        min -= x;
        max -= x;
        return *this;
    }
    BoundingRect<T> Transform(const glm::mat3 & M)
    {
        glm::vec4 p[] =
        {
            M*glm::vec3(min.x, min.y, 1.0f),
            M*glm::vec3(min.x, max.y, 1.0f),
            M*glm::vec3(max.x, min.y, 1.0f),
            M*glm::vec3(max.x, max.y, 1.0f),
        };

        BoundingRect<T> out( vec_type(std::numeric_limits<T>::max()),  vec_type(std::numeric_limits<T>::lowest()) );
        for(int i=0;i<8;i++)
        {
            out.min.x = glm::min( out.min.x, p[i].x );
            out.min.y = glm::min( out.min.y, p[i].y );

            out.max.x = glm::max( out.max.x, p[i].x );
            out.max.y = glm::max( out.max.y, p[i].y );
        }
        return out;
    }

	// Determines if the other AABB intersects this one
	template<typename F>
    bool Intersects(const BoundingRect<F> & other)
	{
		if( other.max.x < min.x || other.min.x > max.x ||
            other.max.y < min.y || other.min.y > max.y  )
		{
			return false;
		}
		return true;
	}

	//
	// Returns true if the other aabb is FULLY contained within this box
	//   
    template<typename F>
    bool Contains(const BoundingRect<F> & other)
	{
		if( other.max.x < max.x &&
			other.min.x > min.x &&
			other.max.y < max.y &&
            other.min.y > min.y )
		{
			return true;
		}
		return false;
	}

	T Volume() const {
        return (max.x-min.x) * (max.y-min.y);
	}
	
	
};

template<typename T>
inline BoundingRect<T> operator-(const BoundingRect<T> & left, const typename BoundingRect<T>::vec_type & x )
{
    return BoundingRect<T>( left.min-x, left.max-x);
}

template<typename T>
inline BoundingRect<T> operator+(const BoundingRect<T> & left, const typename BoundingRect<T>::vec_type & x )
{
    return BoundingRect<T>( left.min+x, left.max+x);
}

template<typename T>
inline BoundingRect<T> operator*(const BoundingRect<T> & left, const T & x )
{
    auto c = left.Centre();
    auto s = left.Size()*0.5f;
    return BoundingRect<T>( c-s*x, c+s*x);
}


}

#endif
