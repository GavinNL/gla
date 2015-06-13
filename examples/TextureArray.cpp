#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <gla/utils/app.h>
#include <gla/utils/cameracontrol.h>
#include <locale>

#include <rgui/rgui.h>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_TITLE "TransformSequence"


using namespace gla;

int main ()
{
    gla::utils::RootApp::Initialize( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);


    //=========================================================================================
    // Enable some openGL stuff
    //=========================================================================================
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS);    // depth-testing interprets a smaller value as "closer"
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //=========================================================================================

    // Load the shaders
    ShaderProgram LineShader(  VertexShader("shaders/Line_PC.v")   , FragmentShader("shaders/Line_PC.f")    );
    ShaderProgram TextureArrayShader( VertexShader("shaders/PNU_TextureArray.v"), FragmentShader("shaders/PNU_TextureArray.f") );

    // Load the objects
    auto Axis  = gla::Solids::createAxes().toGPU();
    auto Box   = gla::Solids::createBox( vec3(5.0,5.0,5.0) ).toGPU();

    // setup the camera
    Camera mCamera;
    mCamera.perspective(45.f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT ,0.2f, 1000.0f);
    mCamera.setPosition( vec3(5.0,1.0,5.0) );

    gla::utils::CameraControl mController( &mCamera );


    //=========================================================================================
    // Create the Texture array on the GPU.
    //   Note: Any objects that start with GPU mean they are initialized on the GPU
    //=========================================================================================
    GPUTextureArray TArray;

    // create an array with 3 layers of 256x256 images
    TArray.create( uvec2(256,256), 3, 2 );

    // Load some textures onto the CPU
    TextureRGBA T1("resources/greyrock.png");
    TextureRGBA T2("resources/rocks.jpg");

    // create a blank texture
    TextureRGBA T3(256,256);

    // set the red channel using a lambda function
    T3.r = [] (vec2 x) {  return glm::perlin( x * 2.0f )*0.5 + 0.5f;  };
    T3.a = 255;

    T1.resize( uvec2(256,256) );
    T2.resize( uvec2(256,256) );

    TArray.SetLayer( T1 , 0);
    TArray.SetLayer( T2 , 1);
    TArray.SetLayer( T3 , 2);



    while ( gla::utils::RootApp::isRunning() )
    {
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //-----------------------------------------------------------------------------
        // Create static variables of some of the uniform locations.
        // This shouldn't be done in practice, it's just convenient for now.
        //-----------------------------------------------------------------------------
        static GLuint TA_ShaderCamMatrixId   = TextureArrayShader.getUniformLocation("inCameraMatrix");
        static GLuint TA_ShaderModelMatrixID = TextureArrayShader.getUniformLocation("inModelMatrix");
        static GLuint TA_ShaderTextureArray  = TextureArrayShader.getUniformLocation("uTextureArray");

        static GLuint LineShaderCamMatrixId   = LineShader.getUniformLocation("inCameraMatrix");
        static GLuint LineShaderModelMatrixID = LineShader.getUniformLocation("inModelMatrix");
        //-----------------------------------------------------------------------------


        TArray.setActiveTexture(0);
        TextureArrayShader.useShader();
            TextureArrayShader.sendUniform_mat4(TA_ShaderCamMatrixId,   mCamera.getProjectionMatrix() * mCamera.getMatrix()  );
            TextureArrayShader.sendUniform_mat4(TA_ShaderModelMatrixID, glm::scale( mat4(1.0), vec3(1.0,1.0,1.0)) );
            TextureArrayShader.sendUniform_Sampler2D(TA_ShaderTextureArray, 0);
        Box.Render();


        LineShader.useShader();
            LineShader.sendUniform_mat4(LineShaderCamMatrixId,   mCamera.getProjectionMatrix() * mCamera.getMatrix()   );
            LineShader.sendUniform_mat4(LineShaderModelMatrixID, glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
        Axis.Render();

        glfwSwapBuffers( gla::utils::RootApp::mWindow );
    }

     return 0;
}

