#include <glre/camera.h>

namespace glre
{

Camera::Camera() : mPosition(0,0,0), mLook(0,0,1), mUp(0,1,0)
{
    mView = M4(1.0f);
    mProj = M4(1.0f);
    mRadius = 100.0;
    mTheta = 3.141592653589 * 0.5;
    mPhi = 0.0;
}





}
