#ifndef GLA_EXP_CAMERA_H
#define GLA_EXP_CAMERA_H

#include "transform.h"

namespace gla { namespace experimental {


class Camera : public Transform
{
    public:
        Camera()
        {

        }

        void Perspective(float FOV, float AspectRatio, float zMin=0.5f, float zMax=10000.f)
        {
             mAspectRatio = AspectRatio;
             mFOV         = FOV;
             mZMin        = zMin;
             mZMax        = zMax;
             mProj        = glm::perspective(FOV, AspectRatio, zMin,zMax);
        }

        mat4 & GetProjectionMatrix() { return mProj; }

        inline mat4 GetMatrix()
        {
            //auto mLook = mTransform.GetOrientation() * vec3(0, 0,-1) + mTransform.GetPosition();
            //auto mUp   = mTransform.GetOrientation() * vec3(0, 1, 0);
            //
            //mView = glm::lookAt( mTransform.GetPosition(), mLook, mUp);

            return glm::scale( mat4(1.0), 1.0f/Scale) * glm::mat4_cast( glm::inverse(Orientation) ) *  glm::translate(mat4(1.0f), -Position);

            //return glm::inverse( Transform::GetMatrix() );
            //return mView;
        }

        vec3 GetDirection()
        {
            //auto mLook = mTransform.GetOrientation() * vec3(0, 0,-1);// + mTransform.getPosition();

            return Orientation * vec3(0, 0,-1);// + mTransform.getPosition();
            //return mLook;
        }

        //vec3 GetPosition() const
        //{
        //    return mTransform.GetPosition();
        //}



        float getFOV() {return mFOV;}
        float getAspectRatio() { return mAspectRatio; }
        float getZMin() { return mZMin;}
        float getZMax() { return mZMax;}

        void  setFOV(float fov)       { Perspective(fov, mAspectRatio,  mZMin, mZMax); }
        void  setAspectRatio(float ar){ Perspective(mFOV, ar         ,  mZMin, mZMax); }
        void  setZMax(float zmax)     { Perspective(mFOV, mAspectRatio, mZMin, zmax); }
        void  setZMin(float zmin)     { Perspective(mFOV, mAspectRatio, zmin, mZMax); }

       // Transform & GetTransform() { return mTransform; }

    private:
            mat4  mProj;
            mat4  mView;

            float mFOV;
            float mAspectRatio;
            float mZMin;
            float mZMax;

};

} }

#endif // CAMERA_H
