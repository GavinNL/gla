#ifndef GLA_CAMERA_H
#define GLA_CAMERA_H

#include <gla/core/global.h>
#include <gla/core/transformation.h>

namespace gla {

class Camera
{
    public:
        Camera()
        {

        }

        inline void perspective(float FOV, float AspectRatio, float zMin=0.5f, float zMax=10000.f)
        {
             mAspectRatio = AspectRatio;
             mFOV         = FOV;
             mZMin        = zMin;
             mZMax        = zMax;
             mProj        = glm::perspective(FOV, AspectRatio, zMin,zMax);
        };

        inline void setPosition( const vec3 & pos )
        {
            mTransform.setPosition(pos);
        }

        inline void translate( const vec3 & pos )
        {
            mTransform.translate( pos );
        }

        inline void setEuler(const vec3 & PitchYawRoll)
        {
             mTransform.setEuler(PitchYawRoll);
        }

        mat4 & getProjectionMatrix() { return mProj; };

        inline mat4 getMatrix()
        {
            auto mLook = mTransform.getOrientation() * vec3(0, 0,-1) + mTransform.getPosition();
            auto mUp   = mTransform.getOrientation() * vec3(0, 1, 0);

            mView = glm::lookAt( mTransform.getPosition(), mLook, mUp);
            return mView;
        }

        float getFOV() {return mFOV;}
        float getAspectRatio() { return mAspectRatio; };
        float getZMin() { return mZMin;}
        float getZMax() { return mZMax;}

        void  setFOV(float fov)       { perspective(fov, mAspectRatio,  mZMin, mZMax); }
        void  setAspectRatio(float ar){ perspective(mFOV, ar         ,  mZMin, mZMax); }
        void  setZMax(float zmax)     { perspective(mFOV, mAspectRatio, mZMin, zmax); }
        void  setZMin(float zmin)     { perspective(mFOV, mAspectRatio, zmin, mZMax); }

        Transformation & getTransform() { return mTransform; };

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
