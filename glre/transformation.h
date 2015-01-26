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
        void translate(const V3 & T)  {mPosition += T;};
        void setPosition(const V3 & P){mPosition  = P;};

        // scaling transformations
        void setScale(const V3 & scale){mScale = scale;}

        // rotational transformation
        void pitch(float delta_radians);
        void roll(float  delta_radians);
        void yaw(float   delta_radians);
        void rotate( const V3 & deltaRollYawPitch );

        // Set the absolute position of the pitch/roll/yaw
        void setPitch(float radians);
        void setRoll(float radians);
        void setYaw(float radians);



        void setEuler( const V3 & RollYawPitch );
        void setEuler(float Pitch, float Roll, float Yaw);

        glre::M4 getMatrix(bool inverse=false);

        const glre::Quat & getOrientation(){ return mOrientation; };
        const glre::V3   & getPosition   (){ return mPosition   ; };
        const glre::V3   & getScale      (){ return mScale; };



    public:
        glre::Quat  mOrientation;
        glre::V3    mPosition;
        glre::V3    mScale;

        glre::V3    mEulerAngles; //pitch,roll,yaw

};

};

#endif // TRANSFORMATION_H
