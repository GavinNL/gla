#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <locale>

#include <gla/utils/app.h>
#include <gla/utils/cameracontrol.h>

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
    void OnWindowPosition(int x, int y)
    {
        std::cout << "Repositioned:  " << x << ", " << y << std::endl;
    }
    void OnWindowSize(int width, int height){}
    void OnFramebufferSize(int width, int height){}
    void OnClose(){mQuit=true;}
    void OnRefresh(){ std::cout << "Refreshed\n"; }
    void OnFocus(int focused){}
    void OnIconify(int iconified){}
    void OnMouseButton(int button, int action, int mods){std::cout << button << std::endl;}
    void OnMousePosition(double x, double y){}
    void OnMouseEnter(int entered){}
    void OnScroll(double x, double y){}
    void OnKey(int key, int scancode, int action, int mods){std::cout << "Key: " << scancode << std::endl;}
    void OnCharacter(unsigned int codepoint){}
    void OnCharacterMods(unsigned int codepoint, int mods){}
    void OnDrop(int count, const char **paths){}

    void run()
    {
        while( !mQuit )
        {
            PollEvents();
            SwapBuffers();
        }
    }

    TransformSequenceApp() : gla::utils::GLFW_App( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
    {
        mQuit = false;
    }

    bool mQuit;
};


int main()
{
    TransformSequenceApp MyApp;

    MyApp.run();

    return 0;

/*
    //===========================================================================
    // This line create the window and initializes GLFW and also
    // creates handles the callbacks.
    //===========================================================================
    gla::utils::RootApp::Initialize( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    //===========================================================================
    // GLA code.
    //===========================================================================
    Timer_T<double> Time;

    // TransformationTimeLine  KeyFrame;
    // KeyFrame.insert(0000, Transformation( {2.0f,0.0f,2.0f}, quat() ) );
    // KeyFrame.insert(2000, Transformation( {2.0f,0.0f,2.0f}, glm::rotate( quat(), 3.0f*3.14159f/2.0f, vec3(0.0,1.0,0.0)) ) );
    // KeyFrame.insert(3000, Transformation( {2.0f,0.0f,2.0f}, quat() ) );

    TransformSequence KeyFrame;
    KeyFrame.setScaleKey(    0.0f, {1.0,1.0,1.0 });
    KeyFrame.setScaleKey(    3.0f, {1.0,1.0,10.0});
    KeyFrame.setScaleKey(    4.0f, {1.0,1.0,1.0 });

    KeyFrame.setPositionKey( 0.0f, {2.0,0.0,2.0} );
    KeyFrame.setPositionKey( 2.0f, {4.0,0.0,2.0} );
    KeyFrame.setPositionKey( 3.0f, {0.0,0.0,4.0} );
    KeyFrame.setPositionKey( 4.0f, {2.0,0.0,2.0} );

    Camera Cam;
    Cam.perspective(45, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT ,0.2f, 1000.0f);
    Cam.setPosition( vec3(2.0,  2.0,  3.0) );
    Cam.setEuler(    vec3(-15,  0.0 , 0.0) * (3.14159f / 180.0f) );

    Transformation Tm;

    // The utils:CameraControl class handles all the
    gla::utils::CameraControl CamController( &Cam );


    auto Axis  = gla::Solids::createAxes().toGPU();


    //---------------------------------------------------------------------------
    //    Create a shader
    //---------------------------------------------------------------------------
    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram LineShader(  VertexShader("shaders/Line_PC.v"),  FragmentShader("shaders/Line_PC.f")  );
    GLuint LineShaderCamMatrixId   = LineShader.getUniformLocation("inCameraMatrix");
    GLuint LineShaderModelMatrixID = LineShader.getUniformLocation("inModelMatrix");

    //==========================================================

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    Timer_T<float> Tf;
    Tf.reset();

    vec3 yrp;
    while ( gla::utils::RootApp::isRunning() )
    {
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //yrp.y += 3.14159f/10.0f * Tf.getElapsedTime();

        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
        LineShader.useShader();
            LineShader.sendUniform_mat4(LineShaderCamMatrixId,   Cam.getProjectionMatrix() * Cam.getMatrix() * Tm.getMatrix()   );
            LineShader.sendUniform_mat4(LineShaderModelMatrixID, glm::scale( mat4(1.0), vec3(10.0,10.0,10.0)) );
        Axis.Render();

       // plane.Render();
       // std::cout << (float)fmod(Time.getElapsedTime(), 3.0)  << std::endl;
        LineShader.sendUniform_mat4(LineShaderCamMatrixId,   Cam.getProjectionMatrix() * Cam.getMatrix()   );
        LineShader.sendUniform_mat4(LineShaderModelMatrixID, KeyFrame.getTransformationMatrix( (float)fmod(Time.getElapsedTime(),4.0f)) );
        //LineShader.sendUniform_mat4(LineShaderModelMatrixID, KeyFrame.get(  static_cast<unsigned int>(Time.getElapsedTime()*1000)%3000).getMatrix() );
        Axis.Render();


        glfwSwapBuffers( gla::utils::RootApp::mWindow );
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    Axis.clear();
    LineShader.DeleteShader();

    gla::utils::RootApp::terminate();
    */
    return 0;

}
