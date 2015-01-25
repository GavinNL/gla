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
        void pitch(float radians);
        void roll(float radians);
        void yaw(float radians);

        void setPitch(float radians);
        void setRoll(float radians);
        void setYaw(float radians);

        void setEuler(float Pitch, float Roll, float Yaw);

        glre::M4 getMatrix();

    public:
        glre::Quat  mOrientation;
        glre::V3    mPosition;
        glre::V3    mScale;

        glre::V3    mEulerAngles; //pitch,roll,yaw
};

};

#endif // TRANSFORMATION_H
