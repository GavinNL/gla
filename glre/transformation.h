/*
 * A transformation class represents a spatial position and an
 * orientation.
 */
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <glre/global.h>

namespace glre
{

class Transformation
{
    public:
        Transformation();

        // positional transformations
        inline void translate(const vec3 & T)  {mPosition += T;};
        inline void setPosition(const vec3 & P){mPosition  = P;};

        // scaling transformations
        inline void setScale(const vec3 & scale){mScale = scale;}

        // rotational transformation
        inline void setOrientation(const quat & q) { mOrientation = q; }
        void inline rotate(const vec3 & axis, float AngleDegrees) { mOrientation = glm::rotate( mOrientation, AngleDegrees, axis ); };


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

        glre::mat4 getMatrix(bool inverse=false);

        const glre::quat   & getOrientation(){ return mOrientation; };
        const glre::vec3   & getPosition   (){ return mPosition   ; };
        const glre::vec3   & getScale      (){ return mScale; };

        quat reverse() const {  return quat(mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z); };

    public:
        glre::quat    mOrientation;
        glre::vec3    mPosition;
        glre::vec3    mScale;

        glre::vec3    mEulerAngles; //pitch,roll,yaw

};

};

#endif // TRANSFORMATION_H
