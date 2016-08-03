#ifndef GLA_BOUNDINGBOX
#define GLA_BOUNDINGBOX

#include <glm/glm.hpp>
#include "boundingrect.h"
#include <algorithm>
#include <gla/transform.h>

namespace gla
{

template<typename T=float>
struct BoundingBox
{
    //using vec_type = glm::detail::tvec3<T, glm::defaultp>;
    using vec_type = glm::tvec3<T, glm::defaultp>;

    vec_type min;
    vec_type max;
	

    BoundingBox( const vec_type & pMin) : min(pMin), max(pMin)
    {
    }

    BoundingBox( const vec_type & pMin, const vec_type & pMax) : min(pMin), max(pMax)
    {

    }

    BoundingBox()
    {

    }

    vec_type Size() const
    {
        return max - min;
    }

    vec_type Centre() const
    {
        return 0.5f*(min + max);
        //return (min + max) / typename vec_type::value_type(2);
    }

    BoundingBox<T>& operator+=(const glm::vec3 & x )
    {
        min += x;
        max += x;
        return *this;
    }
    BoundingBox<T>& operator-=(const glm::vec3 & x )
    {
        min -= x;
        max -= x;
        return *this;
    }


    BoundingBox<T> Transform(const gla::mat4 & M) const
    {
        auto s = this->Size();

        //========== testing =============
        /*
         * assume affine transformation
         */
#if 0
        auto e1  = M * glm::vec4( s.x , 0  ,    0 ,0);
        auto e2  = M * glm::vec4(    0, s.y,    0 ,0);
        auto e3  = M * glm::vec4(    0, 0  ,  s.z ,0);


        // need to construct all the points using min and the 3 axis vectors
        const auto   p7 =           e3;
        const auto & p6 =      e2     ;
              auto   p5 =      e2 + e3;
        const auto & p4 = e1          ;
              auto   p3 = e1 +      e3;
              auto   p2 = e1 + e2     ;
              auto   p1 = e1 + p5     ;



        #error Don't forget to test this!
#endif
        //===================================

        glm::vec4 p[] =
        {
            M*glm::vec4(min.x, min.y, min.z,1.0f),
            M*glm::vec4(min.x, min.y, max.z,1.0f),
            M*glm::vec4(min.x, max.y, min.z,1.0f),
            M*glm::vec4(min.x, max.y, max.z,1.0f),
            M*glm::vec4(max.x, min.y, min.z,1.0f),
            M*glm::vec4(max.x, min.y, max.z,1.0f),
            M*glm::vec4(max.x, max.y, min.z,1.0f),
            M*glm::vec4(max.x, max.y, max.z,1.0f)
        };

        BoundingBox<T> out( glm::vec3(std::numeric_limits<T>::max()),  glm::vec3(std::numeric_limits<T>::lowest()) );

        for(int i=0 ; i < 8 ; i++)
        {
            out.min.x = glm::min( out.min.x, p[i].x );
            out.min.y = glm::min( out.min.y, p[i].y );
            out.min.z = glm::min( out.min.z, p[i].z );

            out.max.x = glm::max( out.max.x, p[i].x );
            out.max.y = glm::max( out.max.y, p[i].y );
            out.max.z = glm::max( out.max.z, p[i].z );
        }

        return out;
    }

    /**
     * @brief Project
     * @param Proj
     * @return BoundingRect of the projection of the
     *
     * Given a matrix that is Proj*View Matrix. Returns a
     * bounding rect that encompases the projection of the boundingbox
     * projected onto the XY plane.
     */
    BoundingRect<T> Project(const glm::mat4 & Proj) const
    {
        BoundingRect<T> out;

        glm::vec4 p[] =
        {
            Proj*glm::vec4(min.x, min.y, min.z,1.0f),
            Proj*glm::vec4(min.x, min.y, max.z,1.0f),
            Proj*glm::vec4(min.x, max.y, min.z,1.0f),
            Proj*glm::vec4(min.x, max.y, max.z,1.0f),
            Proj*glm::vec4(max.x, min.y, min.z,1.0f),
            Proj*glm::vec4(max.x, min.y, max.z,1.0f),
            Proj*glm::vec4(max.x, max.y, min.z,1.0f),
            Proj*glm::vec4(max.x, max.y, max.z,1.0f)
        };


        for(int i=0;i<8;i++)
        {
            float im = 1.0f / p[i].w;
            out.min.x = glm::min( out.min.x, p[i].x*im );
            out.min.y = glm::min( out.min.y, p[i].y*im );

            out.max.x = glm::max( out.max.x, p[i].x*im );
            out.max.y = glm::max( out.max.y, p[i].y*im );
        }


        return out;
    }

    // Determines if the other AABB intersects this one
    template<typename F>
    bool Intersects(const BoundingBox<F> & other)
    {
            if( other.max.x < min.x || other.min.x > max.x ||
                    other.max.y < min.y || other.min.y > max.y ||
                    other.max.z < min.z || other.min.z > max.z  )
            {
                    return false;
            }
            return true;
    }


    // determines where a point p, is relative to the bounding box.
    // in front, back,
    enum class eLocation :unsigned int {
        INSIDE = 0,
        LEFT   = 1,
        RIGHT  = 2,
        UNDER  = 4,
        ABOVE  = 8,
        BACK   = 16,
        FRONT  = 32,

        LEFT_UNDER=  1 | 4,
        LEFT_ABOVE=  1 | 8,

        RIGHT_UNDER  =  2 | 4 ,
        RIGHT_ABOVE  =  2 | 8 ,

        LEFT_BACK   =  1 | 16,
        LEFT_FRONT  =  1 | 32,

        RIGHT_BACK  =  2 | 16,
        RIGHT_FRONT =  2 | 32,


        UNDER_BACK   =  4 | 16,
        UNDER_FRONT  =  4 | 32,
        ABOVE_BACK   =  8 | 16,
        ABOVE_FRONT  =  8 | 32,


        LEFT_UNDER_BACK   =  1 | 4 | 16,
        LEFT_UNDER_FRONT  =  1 | 4 | 32,
        LEFT_ABOVE_BACK   =  1 | 8 | 16,
        LEFT_ABOVE_FRONT  =  1 | 8 | 32,
        RIGHT_UNDER_BACK  =  2 | 4 | 16,
        RIGHT_UNDER_FRONT =  2 | 4 | 32,
        RIGHT_ABOVE_BACK  =  2 | 8 | 16,
        RIGHT_ABOVE_FRONT =  2 | 8 | 32
    };


    eLocation LocatePoint( const glm::vec3 & p )
    {
        //#warning Not Implemented yet.
        unsigned int l=0;

        auto x = p.x < min.x ?  eLocation::LEFT : (p.x > max.x ?  eLocation::RIGHT : eLocation::INSIDE);
        auto y = p.y < min.y ?  eLocation::UNDER: (p.y > max.y ?  eLocation::ABOVE : eLocation::INSIDE);
        auto z = p.z < min.z ?  eLocation::BACK : (p.z > max.z ?  eLocation::FRONT : eLocation::INSIDE);

        l = static_cast<unsigned int>(x) | static_cast<unsigned int>(y) | static_cast<unsigned int>(z);
        return static_cast<eLocation>(l);
    }

    //
    // Returns true if the other aabb is FULLY contained within this box
    //
    bool Contains(const BoundingBox & other) const
    {
            if( other.max.x < max.x &&
                other.min.x > min.x &&
                other.max.y < max.y &&
                other.min.y > min.y &&
                other.max.z < max.z &&
                other.min.z > min.z  )
            {
                    return true;
            }
            return false;
    }

    T Volume() const {
            return (max.x-min.x) * (max.y-min.y) * (max.z-min.z);
    }
	
    float Distance(const glm::vec3 & p)
    {
      float dx = std::max(min.x - p.x, 0, p.x - max.x);
      float dy = std::max(min.y - p.y, 0, p.y - max.y);

      return sqrt(dx*dx + dy*dy);
    }

};

template<typename T>
inline BoundingBox<T> operator-(const BoundingBox<T> & left, const glm::vec3 & x )
{
    return BoundingBox<T>( left.min-x, left.max-x);
}

template<typename T>
inline BoundingBox<T> operator+(const BoundingBox<T> & left, const glm::vec3 & x )
{
    return BoundingBox<T>( left.min+x, left.max+x);
}

template<typename T>
inline BoundingBox<T> operator*(const BoundingBox<T> & left, const T & x )
{
    auto c = left.Centre();
    auto s = left.Size()*0.5f;
    return BoundingBox<T>( c-s*x, c+s*x);
}


}

#endif
