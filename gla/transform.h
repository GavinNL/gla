/*
 * A Transform class represents a spatial position and an
 * orientation.
 */
#ifndef GLA_Transform_H
#define GLA_Transform_H

#include <gla/types.h>

namespace gla
{

/**
 * @brief The Transform class represents the scaling, rotation and translation Transform.
 *
 */
class Transform
{
    public:
        Transform() : Scale(1.0,1.0,1.0), Position(0,0,0), Orientation(1,0,0,0)
        {
        }

        Transform(const vec3 & position, const quat & rot, const vec3 & scale) : Position(position), Scale(scale), Orientation(rot)
        {
        }

        Transform(const vec3 & position, const quat & rot) : Position(position), Scale(1.0f,1.0f,1.0f), Orientation(rot)
        {
        }

        Transform(const vec3 & position) : Position(position), Scale(1,1,1), Orientation(1,0,0,0)
        {

        }

        // positional Transforms
        inline virtual void Translate(const vec3 & T)  { Position += T; }
        inline virtual void SetPosition(const vec3 & P){ Position  = P; }

        // scaling Transforms
        inline virtual void SetScale(const vec3 & scale){Scale = scale;}

        // rotational Transform
        inline virtual void SetOrientation(const quat & q) { Orientation = q; }
        inline virtual void Rotate(const vec3 & axis, float AngleRadians) { Orientation = glm::rotate( Orientation, AngleRadians, axis ); }


        inline void SetEuler( const vec3 & PitchYawRoll )
        {
            Orientation = quat(PitchYawRoll);
        }

        inline virtual gla::mat4 GetMatrix() const
        {
            return glm::translate(mat4(1.0f), Position) * glm::mat4_cast(Orientation) * glm::scale( mat4(1.0), Scale);
        }

        const gla::quat   & GetOrientation() const { return Orientation; }
        const gla::vec3   & GetPosition   () const { return Position;    }
        const gla::vec3   & GetScale      () const { return Scale;       }

        quat Reverse() const {  return quat(Orientation.w, -Orientation.x,  -Orientation.y, -Orientation.z); }

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
            out.Position    = (1.0f-t)*in1.Position + t*in2.Position;
            out.Scale       = (1.0f-t)*in1.Scale    + t*in2.Scale;
            out.Orientation = glm::slerp( in1.Orientation, in2.Orientation, t);
        }

        static Transform Interpolate( Transform & in1, Transform & in2, float t)
        {
            return Transform(
                               (1.0f-t)*in1.Position + t*in2.Position ,
                               glm::slerp( in1.Orientation, in2.Orientation, t) ,
                               (1.0f-t)*in1.Scale    + t*in2.Scale );
        }


    public:
        gla::quat    Orientation;
        gla::vec3    Position;
        gla::vec3    Scale;

        //gla::vec3    mEulerAngles; //pitch,roll,yaw

};



inline Transform operator * (const Transform & ps, const Transform & ls)
{
    Transform w;
    w.Position    = ps.Position  + ps.Orientation * (ps.Scale * ls.Position);
    w.Orientation = ps.Orientation * ls.Orientation;
    w.Scale       = ps.Scale * (ps.Orientation * ls.Scale);

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

    const quat psConjugate( -ps.Orientation.x, -ps.Orientation.y, -ps.Orientation.z, ps.Orientation.w);

    //const quat psConjugate(); ps.Orientation. conjugate(ps.orientation);

    ls.Position    = (psConjugate * (ws.Position - ps.Position)) / ps.Scale;
    ls.Orientation = psConjugate * ws.Orientation;
    ls.Scale       = psConjugate * (ws.Scale / ps.Scale);

    return ls;
}


}

#endif // Transform_H
