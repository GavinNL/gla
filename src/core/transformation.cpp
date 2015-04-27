#include "gla/core/transformation.h"

gla::Transformation::Transformation() : mScale(1.0,1.0,1.0)
{
}

void gla::Transformation::setEuler(const vec3 & PitchYawRoll)
{
   // mEulerAngles = PitchYawRoll;

    // mOrientation =  quat( PitchYawRoll.x , vec3(1.0f,0.0f,0.0f)) *
    //                 quat( PitchYawRoll.y , vec3(0.0f,1.0f,0.0f)) *
    //                 quat( PitchYawRoll.z , vec3(0.0f,0.0f,1.0f));
    //
    // return;

    //vec3 myEuler (fAngle[0],fAngle[1],fAngle[2]);
    mOrientation = quat(PitchYawRoll);
    return;

//    mOrientation =
//                    quat(  cos(PitchYawRoll[0]*0.5 ) , sin(PitchYawRoll[0]*0.5) ,0                       ,  0         )               // Rotate around x axis
//                   *quat(  cos(PitchYawRoll[2]*0.5 ) , 0                        ,0                       ,  sin(PitchYawRoll[2]*0.5)) // Rotate around z axis
//                   *quat(  cos(PitchYawRoll[1]*0.5 ) , 0                        ,sin(PitchYawRoll[1]*0.5),  0         );              // Rotate around y axis
}


gla::mat4 gla::Transformation::getMatrix(bool inverse)
{
    //if(!inverse)
    //return glm::mat4_cast(mOrientation) * glm::translate( glm::scale( mat4(1.0), mScale), mPosition );


    return glm::translate(mat4(1.0f), mPosition) * glm::mat4_cast(mOrientation) * glm::scale( mat4(1.0), mScale);
    //return glm::translate( glm::mat4_cast(mOrientation)*glm::scale(mat4(1.0),mScale), mPosition);
   // return glm::mat4_cast( quat(mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z )) * glm::translate( glm::scale( mat4(1.0), mScale), -mPosition );
}
