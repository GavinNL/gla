#ifndef CAMERA_H
#define CAMERA_H

#include <glre/global.h>

namespace glre {

class Camera
{
    public:


        Camera();


        inline void perspective(float FOV, float AspectRatio, float zMin, float zMax)
        {
            mProj = glm::perspective(FOV, AspectRatio, zMin,zMax);
        };

        inline void set(const V3 & Pos, const V3 & look, const V3 & up)
        {
            mPosition = Pos;
            mLook = look;
            mUp = up;
            updateMatrix();
        };


        inline void lookAt( const V3 & look )
        {
            mLook = look;
            updateMatrix();
        }

        inline void setPosition( const V3 & pos )
        {
            mPosition = pos;
            updateMatrix();
        }

        void updateMatrix()
        {
            mView = glm::lookAt( mPosition, mLook, mUp);

        }

        void updateOrbitMatrix()
        {
            mPosition[0] = mRadius*sin(mTheta)*cos(mPhi);
            mPosition[2] = mRadius*sin(mTheta)*sin(mPhi);
            mPosition[1] = mRadius*cos(mTheta);
            mPosition += mLook;
            updateMatrix();
        }

        void setAngles(float phi, float theta)
        {
            mPhi = phi;
            mTheta = theta;
            updateOrbitMatrix();
        }

        void setRadius(float R)
        {
            mRadius = R;
            updateOrbitMatrix();
        }
        void setPhi(float R)
        {
            mPhi = R;
            updateOrbitMatrix();
        }
        void setTheta(float R)
        {
            mTheta = R;
            updateOrbitMatrix();
        }

        M4 & getProjectionMatrix() { return mProj; };
        M4 & getViewMatrix() {       return mView; };

        M4 mProj;
        M4 mView;

        // for free look
        Quat mOrientation;
        V3 mPosition;
        V3 mLook;
        V3 mUp;


        // For orbiting
        float mTheta;
        float mPhi;
        float mRadius;



};

}
#endif // CAMERA_H
