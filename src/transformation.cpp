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

void glre::Transformation::rotate(const V3 &deltaPitchYawRoll)
{
    setEuler( mEulerAngles + deltaPitchYawRoll);
}

void glre::Transformation::setEuler(const V3 & PitchYawRoll)
{
    mEulerAngles = PitchYawRoll;

    mOrientation =
                    Quat(cos(PitchYawRoll[0]*0.5  ) , sin(PitchYawRoll[0]*0.5) ,0                       ,0         )  // Rotate around x axis
                   *Quat(cos(PitchYawRoll[2]*0.5 )  , 0                        ,0                       ,sin(PitchYawRoll[2]*0.5)) // Rotate around z axis
                   *Quat(cos(PitchYawRoll[1]*0.5   ), 0                        ,sin(PitchYawRoll[1]*0.5),0         );   // Rotate around y axis
}

void glre::Transformation::setEuler(float Roll, float Yaw, float Pitch)
{
    setEuler( V3(Roll,Yaw,Pitch) );
}


glre::M4 glre::Transformation::getMatrix(bool inverse)
{
    if(!inverse)
    return glm::mat4_cast(mOrientation) * glm::translate( glm::scale( M4(1.0), mScale), mPosition );

    return glm::mat4_cast( Quat(mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z )) * glm::translate( glm::scale( M4(1.0), mScale), -mPosition );
}
