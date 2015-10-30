/*
 * A transformation class represents a spatial position and an
 * orientation.
 */
#ifndef GLA_TRANSFORMATION_H
#define GLA_TRANSFORMATION_H

#include <gla/types.h>

namespace gla
{

/**
 * @brief The Transformation class represents the scaling, rotation and translation transformation.
 *
 */
class Transformation
{
    public:
        Transformation() : mScale(1.0,1.0,1.0), mPosition(0,0,0), mOrientation(1,0,0,0)
        {
        }

        Transformation(const vec3 & position, const quat & rot, const vec3 & scale) : mPosition(position), mScale(scale), mOrientation(rot)
        {
        }

        Transformation(const vec3 & position, const quat & rot) : mPosition(position), mScale(1.0f,1.0f,1.0f), mOrientation(rot)
        {
        }

        Transformation(const vec3 & position) : mPosition(position), mScale(1,1,1), mOrientation(1,0,0,0)
        {

        }

        // positional transformations
        inline virtual void translate(const vec3 & T)  { mPosition += T; };
        inline virtual void setPosition(const vec3 & P){ mPosition  = P; };

        // scaling transformations
        inline virtual void setScale(const vec3 & scale){mScale = scale;}

        // rotational transformation
        inline virtual void setOrientation(const quat & q) { mOrientation = q; }
        inline virtual void rotate(const vec3 & axis, float AngleRadians) { mOrientation = glm::rotate( mOrientation, AngleRadians, axis ); };


        inline void setEuler( const vec3 & PitchYawRoll )
        {
            mOrientation = quat(PitchYawRoll);
        }

        inline gla::mat4 getMatrix()
        {
            return glm::translate(mat4(1.0f), mPosition) * glm::mat4_cast(mOrientation) * glm::scale( mat4(1.0), mScale);
        }

        const gla::quat   & getOrientation(){ return mOrientation; };
        const gla::vec3   & getPosition   (){ return mPosition   ; };
        const gla::vec3   & getScale      (){ return mScale; };

        quat reverse() const {  return quat(mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z); };


        /**
         * @brief interpolate
         * @param out  a reference to the transformation object that will be the output
         * @param in1  The initial transformation
         * @param in2  The final transformation
         * @param t    scalar paramter between 0 and 1
         *
         * Interpolates between two transformations.
         */
        static void interpolate( Transformation & out, Transformation & in1, Transformation & in2, float t)
        {
            out.mPosition    = (1.0f-t)*in1.mPosition + t*in2.mPosition;
            out.mScale       = (1.0f-t)*in1.mScale    + t*in2.mScale;
            out.mOrientation = glm::slerp( in1.mOrientation, in2.mOrientation, t);
        }



    public:
        gla::quat    mOrientation;
        gla::vec3    mPosition;
        gla::vec3    mScale;

        //gla::vec3    mEulerAngles; //pitch,roll,yaw

};

//Transform operator*(const Transform& ps, const Transform& ls)
//{
//	Transform ws;

//	ws.position    = ps.position + ps.orientation * (ps.scale * ls.position);
//	ws.orientation = ps.orientation * ls.orientation;
//	ws.scale       = ps.scale * (ps.orientation * ls.scale);

//	return ws;
//}

//Transform& operator*=(Transform& ps, const Transform& ls)
//{
//	ps = ps * ls;

//	return ps;
//}

inline Transformation operator * (const Transformation & ps, const Transformation & ls)
{
    Transformation w;
    w.mPosition    = ps.mPosition  + ps.mOrientation * (ps.mScale * ls.mPosition);
    w.mOrientation = ps.mOrientation * ls.mOrientation;
    w.mScale       = ps.mScale * (ps.mOrientation * ls.mScale);

    return w;

}

inline Transformation& operator *= ( Transformation & ps, const Transformation & ls)
{
    ps = ps * ls;

    return ps;

}


inline Transformation operator/(const Transformation& ws, const Transformation& ps)
{
    Transformation ls;

    const quat psConjugate( -ps.mOrientation.x, -ps.mOrientation.y, -ps.mOrientation.z, ps.mOrientation.w);

    //const quat psConjugate(); ps.mOrientation. conjugate(ps.orientation);

    ls.mPosition    = (psConjugate * (ws.mPosition - ps.mPosition)) / ps.mScale;
    ls.mOrientation = psConjugate * ws.mOrientation;
    ls.mScale       = psConjugate * (ws.mScale / ps.mScale);

    return ls;
}

};

#endif // TRANSFORMATION_H
