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

#ifndef GLA_CAMERA_CONTROL_H
#define GLA_CAMERA_CONTROL_H

#include <gla/core/camera.h>

namespace gla {

template<int Forward=GLFW_KEY_W, int Back=GLFW_KEY_S, int Left=GLFW_KEY_A, int Right=GLFW_KEY_D, int MouseButton=GLFW_MOUSE_BUTTON_RIGHT>
class CameraControl
{
public:
        CameraControl() : mCamera(NULL)
        {
            _xMouse = _yMouse = 0.0;
            _acc    = 100.0f;
            _drag   = 3.30f;
        }

        CameraControl(Camera & C)
        {
            _xMouse = _yMouse = 0.0;
            _acc    = 100.0f;
            _drag   =3.05f;
            attachCamera(C);
        }


        void detachCamera()
        {
            mCamera = NULL;
        }

        void attachCamera( Camera & C)
        {
            mCamera = &C;
            mSpeed = vec3(0.0f);
            mEulerAngles = glm::eulerAngles( C.getTransform().getOrientation() );

        }


        void Calculate(float dt)
        {
            if(mCamera)
            {
                    float       t = dt;
              const float ACC_MAG = _acc;

              // The acceleration vector in worldspace
              vec3 oAcc  = ACC_MAG*( mCamera->getTransform().getOrientation() * mAcc );

              // The drag force is proportional to the velocity of the camera
              vec3 dragForce = -_drag * mSpeed;

             //printf("%f %f %f\n", dragForce.x, dragForce.y, dragForce.z);

              // Calculate the change in velocity based on the acceleration and the drag
              vec3 VelocityChange = (oAcc + dragForce ) * t;

              // Make sure the drag force does not cause the camera to move backwards
              mSpeed     +=  VelocityChange; //glm::length(VelocityChange) > 0.0 ? VelocityChange : vec3(0.0f);

              // Clamp the speed
              float speed = glm::length(mSpeed);


              mSpeed      =   speed >  5.0f ? (5.0f * (mSpeed / speed) ) : mSpeed;

              // Set the new position of the camera
              mCamera->setPosition( mCamera->getTransform().getPosition() + t*mSpeed + 0.0f*oAcc*t*t );
            }
        }

        void InsertMouse(float x, float y)
        {
            float dx = x - _xMouse;
            float dy = y - _yMouse;

            _xMouse = x;
            _yMouse = y;

            if( dx*dx + dy*dy > 30) return;

            if( mLooking && mCamera)
            {
                mEulerAngles.x += dy * 0.0025f;
                mEulerAngles.y -= dx * 0.0025f;

                mEulerAngles = glm::clamp( mEulerAngles, vec3( -3.14159f/180.0f*89 ,-INFINITY, 0), vec3(3.14159f/180.0f*89, INFINITY, 0) );
                mCamera->setEuler(mEulerAngles);
            }
        }


        void InsertButton(int Button, bool Down)
        {
            if(Button == MouseButton)
            {
                mLooking = Down;
            }
        }


        void InsertKey( int KeyCode, bool Down )
        {
            if( !Down) mAcc *= 0.0f;
            switch(KeyCode)
            {
                case Forward:
                    mAcc[2] = Down ? -1.0f : 0.0f;
                    break;
                case Back:
                    mAcc[2] = Down ? 1.0f : 0.0f;
                    break;
                case Left:
                    mAcc[0] = Down ?  -1.0f : 0.0f;
                    break;
                case Right:
                    mAcc[0] = Down ? 1.0f : 0.0f;
                    break;
            }
        }


        public:
            float _drag;
            float _acc;

        private:
            Camera * mCamera;

            float _xMouse;
            float _yMouse;

            bool mLooking;
            vec3 mAcc;
            vec3 mSpeed;
            vec3 mEulerAngles;
};


} //glre

#endif // EVENTS_H



