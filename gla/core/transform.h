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


/*
 * A Transform class represents a spatial position and an
 * orientation.
 */
#ifndef GLA_EXP_Transform_H
#define GLA_EXP_Transform_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

namespace gla {

/**
 * @brief The Transform class represents the scaling, rotation and translation Transform.
 *
 */
class Transform
{
    public:



        Transform() : Position(0,0,0) , Orientation(1,0,0,0) , Scale(1.0,1.0,1.0)
        {
        }

        Transform(const glm::vec3 & position, const glm::quat & rot, const glm::vec3 & scale) : Position(position), Orientation(rot), Scale(scale)
        {
        }

        Transform(const glm::vec3 & position, const glm::quat & rot) : Position(position), Orientation(rot), Scale(1.0f,1.0f,1.0f)
        {
        }

        Transform(const glm::vec3 & position) : Position(position), Orientation(1,0,0,0), Scale(1,1,1)
        {

        }

        // positional Transforms
        inline virtual void Translate(const glm::vec3 & T)  { Position += T; }
        inline virtual void SetPosition(const glm::vec3 & P){ Position  = P; }

        // scaling Transforms
        inline virtual void SetScale(const glm::vec3 & scale){Scale = scale;}

        // rotational Transform
        inline virtual void SetOrientation(const glm::quat & q) { Orientation = q; }
        inline virtual void Rotate(const glm::vec3 & axis, float AngleRadians) { Orientation = glm::rotate( Orientation, AngleRadians, axis ); }

        inline void SetEuler( const glm::vec3 & PitchYawRoll )
        {
            Orientation = glm::quat(PitchYawRoll);
        }

        inline virtual glm::mat4 GetMatrix() const
        {
            return glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(Orientation) * glm::scale( glm::mat4(1.0), Scale);
        }

        const glm::quat   & GetOrientation() const { return Orientation; }
        const glm::vec3   & GetPosition   () const { return Position;    }
        const glm::vec3   & GetScale      () const { return Scale;       }

        glm::quat Reverse() const {  return glm::quat(Orientation.w, -Orientation.x,  -Orientation.y, -Orientation.z); }

        Transform Inverse() const
        {
            return Transform( -Position, Reverse(), 1.0f / Scale );
        }

        /**
         * @brief interpolate
         * @param out  a reference to the Transform object that will be the output
         * @param in1  The initial Transform
         * @param in2  The final Transform
         * @param t    scalar paramter between 0 and 1
         *
         * Interpolates between two Transforms.
         */
        static void Interpolate( Transform & out, Transform & in1, Transform & in2, float t)
        {
            auto omt = 1.0f-t;
            out.Position    = omt*in1.Position + t*in2.Position;
            out.Scale       = omt*in1.Scale    + t*in2.Scale;
            out.Orientation = glm::slerp( in1.Orientation, in2.Orientation, t);
        }

        static Transform Interpolate( const Transform & in1, const Transform & in2, float t)
        {
            return Transform(
                               (1.0f-t)*in1.Position + t*in2.Position ,
                               glm::slerp( in1.Orientation, in2.Orientation, t) ,
                               (1.0f-t)*in1.Scale    + t*in2.Scale );
        }


    public:
        glm::vec3    Position;
        glm::quat    Orientation;
        glm::vec3    Scale;

        //glm::vec3    mEulerAngles; //pitch,roll,yaw

};



inline Transform operator * (const Transform & ps, const Transform & ls)
{
    Transform w;
    w.Position    = ps.Position  + ps.Orientation * (ps.Scale * ls.Position);
    w.Orientation = ps.Orientation * ls.Orientation;
    //w.Scale       = ps.Scale * (ps.Orientation * ls.Scale);
    w.Scale       = ps.Scale * ls.Scale;

    return w;

}

inline Transform& operator *= ( Transform & ps, const Transform & ls)
{
    ps = ps * ls;

    return ps;

}


inline Transform operator/(const Transform& ws, const Transform& ps)
{
    Transform ls;

    const glm::quat psConjugate( ps.Orientation.w, -ps.Orientation.x, -ps.Orientation.y, -ps.Orientation.z);

    //const glm::quat psConjugate(); ps.Orientation. conjugate(ps.orientation);

    ls.Position    = (psConjugate * (ws.Position - ps.Position)) / ps.Scale;
    ls.Orientation = psConjugate * ws.Orientation;
    ls.Scale       = psConjugate * (ws.Scale / ps.Scale);

    return ls;
}


}

#endif // Transform_H
