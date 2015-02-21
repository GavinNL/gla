#ifndef CAMERA_H
#define CAMERA_H

#include <glre/global.h>
#include <glre/transformation.h>

namespace glre {

class Camera : public Transformation
{
    public:


        Camera()
        {
        }


        inline void perspective(float FOV, float AspectRatio, float zMin, float zMax)
        {
             mAspectRatio = AspectRatio;
             mFOV   = FOV;
             mZMin  = zMin;
             mZMax  = zMax;
             mProj  = glm::perspective(FOV, AspectRatio, zMin,zMax);
        };

        inline void setPosition( const vec3 & pos )
        {
            this->Transformation::setPosition( -pos );
        }

        inline void translate( const vec3 & pos )
        {
            this->Transformation::translate( pos );
        }

        inline void moveTowardOrientation( const vec3 & displacement )
        {
            this->Transformation::translate( quat( mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z) * displacement );
        }

        inline vec3 getDirection(const vec3 Forward = vec3(0.0,0.0,1.0) )
        {
            return quat( mOrientation.w, -mOrientation.x, -mOrientation.y, -mOrientation.z) * Forward;
        }

        mat4 & getProjectionMatrix() { return mProj; };


        float getFOV() {return mFOV;}
        float getAspectRatio() { return mAspectRatio; };
        float getZMin() { return mZMin;}
        float getZMax() { return mZMax;}

        void setFOV(float fov) { perspective(fov, mAspectRatio, mZMin, mZMax); }
        void setAspectRatio(float aspectratio) { perspective(mFOV, aspectratio, mZMin, mZMax); }
        void setZMax(float zmax) { perspective(mFOV, mAspectRatio, mZMin, zmax); }
        void setZMin(float zmin) { perspective(mFOV, mAspectRatio, zmin, mZMax); }


    private:
            mat4  mProj;

            float mFOV;
            float mAspectRatio;
            float mZMin;
            float mZMax;

};

}
#endif // CAMERA_H
