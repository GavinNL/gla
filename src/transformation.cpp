#include "glre/transformation.h"

glre::Transformation::Transformation() : mScale(1.0,1.0,1.0)
{
}

void glre::Transformation::yaw(float radians)
{
    mOrientation = quat( cos(radians*0.5), 0, sin(radians*0.5),0) * mOrientation;
}

void glre::Transformation::pitch(float radians)
{
    mOrientation = quat( cos(radians*0.5), sin(radians*0.5), 0, 0 ) * mOrientation;
}

void glre::Transformation::roll(float radians)
{
    mOrientation = quat(cos(radians*0.5), 0, 0, sin(radians*0.5)) * mOrientation;
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

void glre::Transformation::rotate(const vec3 &deltaPitchYawRoll)
{
    setEuler( mEulerAngles + deltaPitchYawRoll);
}

void glre::Transformation::setEuler(const vec3 & PitchYawRoll)
{
    mEulerAngles = PitchYawRoll;

    mOrientation =
                    quat(cos(PitchYawRoll[0]*0.5  ) , sin(PitchYawRoll[0]*0.5) ,0                       ,0         )  // Rotate around x axis
                   *quat(cos(PitchYawRoll[2]*0.5 )  , 0                        ,0                       ,sin(PitchYawRoll[2]*0.5)) // Rotate around z axis
                   *quat(cos(PitchYawRoll[1]*0.5   ), 0                        ,sin(PitchYawRoll[1]*0.5),0         );   // Rotate around y axis
}

void glre::Transformation::setEuler(float Roll, float Yaw, float Pitch)
{
    setEuler( vec3(Roll,Yaw,Pitch) );
}


glre::mat4 glre::Transformation::getMatrix(bool inverse)
{
    //if(!inverse)
    return glm::mat4_cast(mOrientation) * glm::translate( glm::scale( mat4(1.0), mScale), mPosition );

   // return glm::mat4_cast( quat(mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z )) * glm::translate( glm::scale( mat4(1.0), mScale), -mPosition );
}
