#ifndef GLA_CAMERA_CONTROL_H
#define GLA_CAMERA_CONTROL_H


#include <gla/core/camera.h>

namespace gla {
namespace utils {

template<int Forward, int Back, int Left, int Right, int MouseButton>
class CameraControl
{
public:
        CameraControl( Camera * C)
        {
            if(C) attachCamera(C);
            _xMouse = _yMouse = 0.0;
            _acc    = 500.0f;
            _drag   = 0.05f;
        }


        void detachCamera()
        {
            mCamera = NULL;
        }

        void attachCamera( Camera * C)
        {
//            using namespace std::placeholders;
            mCamera = C;
            mSpeed = vec3(0.0f);
            mEulerAngles = glm::eulerAngles( C->getTransform().getOrientation() );

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

             // printf("%f %f %f\n", oAcc.x, oAcc.y, oAcc.z);

              // Calculate the change in velocity based on the acceleration and the drag
              vec3 VelocityChange = (oAcc + dragForce ) * t;

              // Make sure the drag force does not cause the camera to move backwards
              mSpeed     +=  glm::length(VelocityChange) > 0.0 ? VelocityChange : vec3(0.0f);

              // Clamp the speed
              float speed = glm::length(mSpeed);
              mSpeed      =   mSpeed  * glm::min( 5.0f, speed ) / ( speed + 0.001f);

              // Set the new position of the camera
              mCamera->setPosition( mCamera->getTransform().getPosition() + t*mSpeed + 0.5f*oAcc*t*t );
            }
        }

        void InsertMouse(float x, float y)
        {
            float dx = x - _xMouse;
            float dy = y - _yMouse;

            _xMouse = x;
            _yMouse = y;

            if( mLooking )
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
            printf("Keycode: %d  %d\n", KeyCode, Forward);
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

        /*
        void InputCallback (gla::utils::InputEvent & M)
        {
            using namespace gla::utils;

            switch( M.type )
            {
                case InputEvent::FRAME:
                    {
                              float       t = M.Frame.dt;
                        const float ACC_MAG = 500.0f;
                               vec3     acc;

                        acc[2] = -( RootApp::isKeyDown(GLFW_KEY_W) ? 1.0f : RootApp::isKeyDown(GLFW_KEY_S) ? -1.0f : 0.0f );
                        acc[0] = -( RootApp::isKeyDown(GLFW_KEY_A) ? 1.0f : RootApp::isKeyDown(GLFW_KEY_D) ? -1.0f : 0.0f );

                        vec3 oAcc  = ACC_MAG*( mCamera->getTransform().getOrientation()*acc )*t;

                        const float DRAG = 0.05;

                        vec3 dragForce = glm::length(oAcc) > 1.e-3 ? vec3(0.0f) : (-DRAG * mSpeed);

                        mSpeed     += (oAcc + dragForce ) * t;
                        mSpeed      =   mSpeed  * glm::min( 5.0f, glm::length(mSpeed) ) / ( glm::length(mSpeed) + 0.001f);

                        printf("%f\n", mSpeed[0]);

                        mCamera->setPosition( mCamera->getTransform().getPosition() + t*mSpeed + 0.5f*oAcc*t*t );
                    }
                    break;
                case InputEvent::MOUSE:
                    if( RootApp::isButtonDown(GLFW_MOUSE_BUTTON_RIGHT) )
                    {
                        mEulerAngles.x += M.Mouse.dy * 0.0025f;
                        mEulerAngles.y -= M.Mouse.dx * 0.0025f;

                        mEulerAngles = glm::clamp( mEulerAngles, vec3( -3.14159f/180.0f*89 ,-INFINITY, 0), vec3(3.14159f/180.0f*89, INFINITY, 0) );

                        mCamera->setEuler(mEulerAngles);

                    }
                    break;
                case InputEvent::BUTTON:
                    if( M.Button.button == GLFW_MOUSE_BUTTON_RIGHT)
                    {
                        glfwSetInputMode(RootApp::mWindow, GLFW_CURSOR, M.Button.action ? GLFW_CURSOR_DISABLED:  GLFW_CURSOR_NORMAL );
                    }

                    break;
            }

        };
        */

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


} //utils
} //glre

#endif // EVENTS_H



