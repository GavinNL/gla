/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GLA_EXP_CAMERA_H
#define GLA_EXP_CAMERA_H

#include "transform.h"

namespace gla {


class Camera : public Transform
{
    public:
        Camera()
        {

        }

        void Perspective( float FOV, float AspectRatio, float zMin=0.5f, float zMax=10000.f)
        {
             mAspectRatio = AspectRatio;
             mFOV         = FOV;
             mZMin        = zMin;
             mZMax        = zMax;
             mProj        = glm::perspective(FOV, AspectRatio, zMin,zMax);
        }

        glm::mat4 & GetProjectionMatrix() { return mProj; }

        inline glm::mat4 GetMatrix() const override
        {
            //auto mLook = mTransform.GetOrientation() * vec3(0, 0,-1) + mTransform.GetPosition();
            //auto mUp   = mTransform.GetOrientation() * vec3(0, 1, 0);
            //
            //mView = glm::lookAt( mTransform.GetPosition(), mLook, mUp);

            return glm::scale( glm::mat4(1.0), 1.0f/Scale) * glm::mat4_cast( glm::inverse(Orientation) ) *  glm::translate(glm::mat4(1.0f), -Position);

            //return glm::inverse( Transform::GetMatrix() );
            //return mView;
        }

        glm::vec3 GetDirection()
        {
            //auto mLook = mTransform.GetOrientation() * vec3(0, 0,-1);// + mTransform.getPosition();

            return Orientation * glm::vec3(0, 0,-1);// + mTransform.getPosition();
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
            glm::mat4  mProj;
            glm::mat4  mView;

            float mFOV;
            float mAspectRatio;
            float mZMin;
            float mZMax;

};

}

#endif // CAMERA_H
