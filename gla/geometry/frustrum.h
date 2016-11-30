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

#ifndef GLA_FRUSTRUM_H
#define GLA_FRUSTRUM_H

#include "boundingbox.h"
#include "line3d.h"


namespace gla {
/*
    Frustum class that represents the frustum in 3 dimensional space.
    it can be used to check clipping with bounding boxes.

*/
struct Frustum
{
    Frustum()
    {

    }


    //Frustum(const gla::Camera & camera) : Frustum( camera.GetProjectionMatrix() )
    //{
    //    Transform( camera.GetMatrix() );
    //}

    /**
     * @brief Frustum
     * @param proj
     *
     * Constructs the frustrum from a perspective projection matrix
     */
    Frustum(const glm::mat4 & proj)
    {
        const auto & m = proj;
        left.v.x = -(m[0][3] + m[0][0]);
        left.v.y = -(m[1][3] + m[1][0]);
        left.v.z = -(m[2][3] + m[2][0]);
        left.p   = glm::vec3(0.0f);
        left.v = glm::normalize(left.v);

        // right
        right.v.x = -(m[0][3] - m[0][0]);
        right.v.y = -(m[1][3] - m[1][0]);
        right.v.z = -(m[2][3] - m[2][0]);
        right.p = glm::vec3(0.0f);
        right.v = glm::normalize(right.v);

        // bottom
        bottom.v.x = -(m[0][3] + m[0][1]);
        bottom.v.y = -(m[1][3] + m[1][1]);
        bottom.v.z = -(m[2][3] + m[2][1]);
        bottom.p = glm::vec3(0.0f);
        bottom.v = glm::normalize(bottom.v);

        // top
        top.v.x = -(m[0][3] - m[0][1]);
        top.v.y = -(m[1][3] - m[1][1]);
        top.v.z = -(m[2][3] - m[2][1]);
        top.p   = glm::vec3(0.0f);
        top.v = glm::normalize(top.v);

        // near
        near.v.x = m[0][3] + m[0][2];
        near.v.y = m[1][3] + m[1][2];
        near.v.z = m[2][3] + m[2][2];
        near.p = glm::vec3(0,0, -(m[3][3] + m[3][2])/near.v.z);
        near.v = -glm::normalize(near.v);

        // far
        far.v.x = m[0][3] - m[0][2];
        far.v.y = m[1][3] - m[1][2];
        far.v.z = m[2][3] - m[2][2];
        far.p = glm::vec3(0,0, -(m[3][3] - m[3][2]) / far.v.z);
        far.v = -glm::normalize(far.v);
    }

    // Transform the fustrum using a matrix
    // The matrix M needs to be unitary
    void Transform(const glm::mat4 & ViewMatrix)
    {
        gla::Line3d * L = &top;

        for(int i=0;i<6;i++)
        {
            auto p4 = ViewMatrix * glm::vec4( L[i].p.x, L[i].p.y, L[i].p.z, 1.0f);
            auto v4 = ViewMatrix * glm::vec4( L[i].v.x, L[i].v.y, L[i].v.z, 0.0f);

            L[i].p = glm::vec3(p4.x,p4.y,p4.z);
            L[i].v = glm::vec3(v4.x,v4.y,v4.z);
        }

        auto p4 = ViewMatrix * glm::vec4( p.x, p.y, p.z, 1.0f);
        p = glm::vec3(p4.x,p4.y,p4.z);
    }


    /**
     * @brief Intersects
     * @param B
     * @return
     *
     * Determines if a bounding box intersects the fustrum
     */
    template<typename T>
    bool Intersects(const gla::BoundingBox<T> & B) const
    {
        // for each plane, check if the corners of the bounding box are on the same side of the plane
        gla::Line3d const * L = &top;

        const glm::vec3 P[] =
        {
            glm::vec3( B.min.x, B.min.y, B.min.z),
            glm::vec3( B.min.x, B.min.y, B.max.z),
            glm::vec3( B.min.x, B.max.y, B.min.z),
            glm::vec3( B.min.x, B.max.y, B.max.z),
            glm::vec3( B.max.x, B.min.y, B.min.z),
            glm::vec3( B.max.x, B.min.y, B.max.z),
            glm::vec3( B.max.x, B.max.y, B.min.z),
            glm::vec3( B.max.x, B.max.y, B.max.z)
        };

        // for each plane
        for(int i=0 ; i<6 ; i++)
        {
            int c = 0;
            c += (glm::dot( P[0] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;
            c += (glm::dot( P[1] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;
            c += (glm::dot( P[2] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;
            c += (glm::dot( P[3] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;
            c += (glm::dot( P[4] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;
            c += (glm::dot( P[5] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;
            c += (glm::dot( P[6] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;
            c += (glm::dot( P[7] -L[i].p, L[i].v) / glm::length(L[i].v) ) > 0;

            if(c==8) return false;
        }
        return true;
    }


    /**
     * @brief Intersects
     * @param point
     * @return true if the point is within the furstrum
     */
    bool Intersects(const glm::vec3 & P ) const
    {
        gla::Line3d const * L = &top;

        for(int i=0;i<6;i++)
        {
            const float d = glm::dot( P-L[i].p, L[i].v) / glm::length(L[i].v);
            if( d > 0) return false;
        }
        return true;
    }



    const glm::vec3 & GetPosition() const
    {
        return p;
    }

protected:
    glm::vec3 p; // position of the camera in 3d space. The position of the point of the pyrimad

    // the 6 planes that define the fustrum
    // these planes are in real coordinate space. not normalized.
    gla::Line3d top;    // this plane passes through the point, p,
    gla::Line3d right;  // this plane passes through the point, p,
    gla::Line3d bottom; // this plane passes through the point, p,
    gla::Line3d left;   // this plane passes through the point, p,
    gla::Line3d near;
    gla::Line3d far;

};

}

#endif

