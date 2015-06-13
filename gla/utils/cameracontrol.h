#ifndef GLA_CAMERA_CONTROL_H
#define GLA_CAMERA_CONTROL_H

#include <GLFW/glfw3.h>
#include <gla/utils/app.h>
#include <gla/core/camera.h>
#include <functional>

namespace gla {
namespace utils {

class CameraControl
{
public:
        CameraControl( Camera * C)
        {
            attachCamera(C);
        }


        void detachCamera()
        {
            char Name[50];
            sprintf(Name, "CameraControl_%p", this);
            gla::utils::RootApp::InputEvents.erase(Name);
        }

        void attachCamera( Camera * C)
        {
            using namespace std::placeholders;
            mCamera = C;
            mSpeed = vec3(0.0f);
            mEulerAngles = glm::eulerAngles( C->getTransform().getOrientation() );

            char Name[50];
            sprintf(Name, "CameraControl_%p", this);
            gla::utils::RootApp::InputEvents[Name]
                    = std::bind( &CameraControl::InputCallback, this, _1 );
        }

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

                        const float DRAG = 0.01;

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

        Camera * mCamera;

        vec3 mSpeed;
        vec3 mEulerAngles;
};


} //utils
} //glre

#endif // EVENTS_H



