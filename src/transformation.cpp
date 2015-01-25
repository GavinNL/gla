#include "glre/transformation.h"

glre::Transformation::Transformation() : mScale(1.0,1.0,1.0)
{
}

void glre::Transformation::yaw(float radians)
{

    mOrientation = Quat(cos(radians*0.5), 0, sin(radians*0.5),0) * mOrientation;
}

void glre::Transformation::setRoll(float radians)
{
    setEuler( mEulerAngles[0], radians, mEulerAngles[2]);
}

void glre::Transformation::setYaw(float radians)
{
    setEuler( mEulerAngles[0], mEulerAngles[1], radians);
}

void glre::Transformation::setPitch(float radians)
{
    setEuler( radians, mEulerAngles[1], mEulerAngles[2]);
}

void glre::Transformation::setEuler(float Pitch, float Roll, float Yaw)
{

    mEulerAngles[0] = Pitch;
    mEulerAngles[1] = Roll;
    mEulerAngles[2] = Yaw;

    mOrientation =
                    Quat(cos(Pitch*0.5 ), 0             ,0           ,sin(Pitch*0.5)) // Rotate around z axis
                   *Quat(cos(Yaw*0.5   ), 0             ,sin(Yaw*0.5),0         )   // Rotate around y axis
                   *Quat(cos(Roll*0.5  ), sin(Roll*0.5) ,0           ,0         ); // Rotate around x axis
}


glre::M4 glre::Transformation::getMatrix()
{
//myTranslationMatrix * myRotationMatrix * myScaleMatrix;
return glm::translate( glm::mat4_cast(mOrientation) * glm::scale( M4(1.0), mScale) , mPosition );
}
