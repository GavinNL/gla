#ifndef GLA_CAMERA_H
#define GLA_CAMERA_H

#include <gla/global.h>
#include <gla/transformation.h>

namespace gla
{

class Camera
{
    public:
        Camera()
        {

        }

        inline void Perspective(float FOV, float AspectRatio, float zMin=0.5f, float zMax=10000.f)
        {
             mAspectRatio = AspectRatio;
             mFOV         = FOV;
             mZMin        = zMin;
             mZMax        = zMax;
             mProj        = glm::perspective(FOV, AspectRatio, zMin,zMax);
        }

        inline void SetPosition( const vec3 & pos )
        {
            mTransform.SetPosition( pos );
        }

        inline void Translate( const vec3 & pos )
        {
            mTransform.Translate( pos );
        }

        inline void SetEuler(const vec3 & PitchYawRoll)
        {
             mTransform.SetEuler(PitchYawRoll);
        }

        mat4 & GetProjectionMatrix() { return mProj; }

        inline mat4 GetMatrix()
        {
            auto mLook = mTransform.GetOrientation() * vec3(0, 0,-1) + mTransform.GetPosition();
            auto mUp   = mTransform.GetOrientation() * vec3(0, 1, 0);

            mView = glm::lookAt( mTransform.GetPosition(), mLook, mUp);
            return mView;
        }

        vec3 GetDirection()
        {
            auto mLook = mTransform.GetOrientation() * vec3(0, 0,-1);// + mTransform.getPosition();
            return mLook;
        }

        vec3 GetPosition() const
        {
            return mTransform.GetPosition();
        }



        float getFOV() {return mFOV;}
        float getAspectRatio() { return mAspectRatio; }
        float getZMin() { return mZMin;}
        float getZMax() { return mZMax;}

        void  setFOV(float fov)       { Perspective(fov, mAspectRatio,  mZMin, mZMax); }
        void  setAspectRatio(float ar){ Perspective(mFOV, ar         ,  mZMin, mZMax); }
        void  setZMax(float zmax)     { Perspective(mFOV, mAspectRatio, mZMin, zmax); }
        void  setZMin(float zmin)     { Perspective(mFOV, mAspectRatio, zmin, mZMax); }

        Transformation & GetTransform() { return mTransform; }

    private:
            mat4  mProj;
            mat4  mView;

            Transformation mTransform;

            float mFOV;
            float mAspectRatio;
            float mZMin;
            float mZMax;

};

}

#endif // CAMERA_H
