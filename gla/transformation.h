/*
 * A transformation class represents a spatial position and an
 * orientation.
 */
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <gla/global.h>

namespace gla
{

class Transformation
{
    public:
        Transformation();

        // positional transformations
        inline virtual void translate(const vec3 & T)  {mPosition += T;};
        inline virtual void setPosition(const vec3 & P){mPosition  = P;};

        // scaling transformations
        inline virtual void setScale(const vec3 & scale){mScale = scale;}

        // rotational transformation
        inline virtual void setOrientation(const quat & q) { mOrientation = q; }
        inline virtual void rotate(const vec3 & axis, float AngleRadians) { mOrientation = glm::rotate( mOrientation, AngleRadians, axis ); };


        void pitch(float delta_radians);
        void roll(float  delta_radians);
        void yaw(float   delta_radians);
        void rotate( const vec3 & deltaRollYawPitch );

        // Set the absolute position of the pitch/roll/yaw
        void setPitch(float radians);
        void setRoll(float radians);
        void setYaw(float radians);



        void setEuler( const vec3 & RollYawPitch );
        void setEuler(float Pitch, float Roll, float Yaw);

        gla::mat4 getMatrix(bool inverse=false);

        const gla::quat   & getOrientation(){ return mOrientation; };
        const gla::vec3   & getPosition   (){ return mPosition   ; };
        const gla::vec3   & getScale      (){ return mScale; };

        quat reverse() const {  return quat(mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z); };

    public:
        gla::quat    mOrientation;
        gla::vec3    mPosition;
        gla::vec3    mScale;

        gla::vec3    mEulerAngles; //pitch,roll,yaw

};

};

#endif // TRANSFORMATION_H
