#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <locale>

#include <gla/utils/app.h>
#include <gla/engine/cameracontrol.h>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_TITLE "TransformSequence"


class TransformSequenceApp : public gla::utils::GLFW_App
{
        // RootApp interface
    public:

        // Some unused callback functions. We don't really need them.
        void OnWindowSize(int width, int height){}
        void OnFramebufferSize(int width, int height){}
        void OnRefresh(){ }
        void OnFocus(int focused){}
        void OnIconify(int iconified){}
        void OnMouseEnter(int entered){}
        void OnScroll(double x, double y){}
        void OnCharacter(unsigned int codepoint){}
        void OnCharacterMods(unsigned int codepoint, int mods){}
        void OnDrop(int count, const char **paths){}
        void OnWindowPosition(int x, int y){}




        void OnClose()
        {
            mQuit=true;
        }


        // Send the keyboard/mouse input to the camera controller
        void OnMouseButton(int button, int action, int mods)
        {
            mCamControl.InsertButton(button, action);
        }

        void OnMousePosition(double x, double y)
        {
            mCamControl.InsertMouse( (float)x, (float)y );
        }

        void OnKey(int key, int scancode, int action, int mods)
        {
            mCamControl.InsertKey( key, action );
        }

        void run()
        {
            double t;
            while( !mQuit )
            {

                float dt = mTimer.getElapsedTime(); mTimer.reset();
                t += dt;

                glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                // The camera controller needs to know how much time has passed since
                // the last frame. This is for the acceleration calculations and such
                mCamControl.Calculate( dt );



                // Activate the line shader
                LineShader.useShader();

                // Send the Camera Matrix and the Model matrix for the main Axis.
                LineShader.sendUniform(0, "inCameraMatrix", mCamera.getProjectionMatrix() * mCamera.getMatrix()    );
                LineShader.sendUniform(1, "inModelMatrix" , glm::scale( mat4(1.0), vec3(10.0,10.0,10.0))  );

                // Render the axis
                mAxis.Render();


                // Compute the transformation matrix from the KeyFrames. We want the transformation sequence
                // to repeat which is why we fmod the elapsed time with 4.
                // The getTransformationMatrix will interpolate the keyframes to produce a new matrix
                // that represents the transformation
                auto TransformationMatrix = KeyFrame.getTransformationMatrix( (float)fmod(t,4.0f) );

                // Send the model matrix for the secondary axis
                LineShader.sendUniform(1, "inModelMatrix" , TransformationMatrix );

                // render the axis again.
                mAxis.Render();



                PollEvents();
                SwapBuffers();
            }
        }




        TransformSequenceApp() : gla::utils::GLFW_App( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
        {
            // Some initial setup
            mQuit = false;
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


            // Set up the camera.
            mCamera.perspective(45, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT ,0.2f, 1000.0f);
            mCamera.setPosition( vec3(2.0,  2.0,  3.0) );
            mCamera.setEuler(    vec3(-15,  0.0 , 0.0) * (3.14159f / 180.0f) );

            // attack the camera to the camera controller
            mCamControl.attachCamera(mCamera);

            // compile the shader
            LineShader.linkProgram( VertexShader("shaders/Line_PC.v"),  FragmentShader("shaders/Line_PC.f") );


            // Set the Key Frames. The first parameter is the time and the second paramter is the value the key should be.
            KeyFrame.setScaleKey(    0.0f, {1.0,1.0,1.0 });
            KeyFrame.setScaleKey(    3.0f, {3.0,3.0,3.0 });
            KeyFrame.setScaleKey(    4.0f, {1.0,1.0,1.0 });

            KeyFrame.setPositionKey( 0.0f, {2.0,0.0,2.0} );
            KeyFrame.setPositionKey( 2.0f, {4.0,0.0,2.0} );
            KeyFrame.setPositionKey( 3.0f, {0.0,0.0,4.0} );
            KeyFrame.setPositionKey( 4.0f, {2.0,0.0,2.0} );

            KeyFrame.setRotKey( 0.0f, quat( vec3(0, 0,0) ) );         // pitch,yaw,roll = 0
            KeyFrame.setRotKey( 2.0f, quat( vec3(0, 3.14159,0 )) );   // face 180 degrees
            KeyFrame.setRotKey( 4.0f, quat( vec3(0, 2*3.14159,0 )) ); // face 360 degrees

            mAxis  = gla::Solids::createAxes().toGPU();
        }

        bool                      mQuit;
        gla::CameraControl<GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_MOUSE_BUTTON_RIGHT> mCamControl;
        gla::Camera               mCamera;
        gla::Timer_T<double>      mTimer;


        TransformSequence         KeyFrame;



        //--------------- Shaders -----------------------------
        ShaderProgram LineShader;


        //--------------- GPU Objects -------------------------
        GPUArrayObject mAxis;


};



int main()
{
    TransformSequenceApp MyApp;

    MyApp.run();

    return 0;

}
