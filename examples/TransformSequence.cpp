#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <locale>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    //===========================================================================
    // GLA code.
    //===========================================================================
    Timer_T<double> Time;

    TransformSequence T;
    T.setPositionKey(0.0f,  vec3(1.0f,0.0f,1.0f));
    T.setPositionKey(1.0f,  vec3(2.0f,0.0f,1.0f));
    T.setPositionKey(2.0f,  vec3(2.0f,0.0f,2.0f));
    T.setPositionKey(3.0f,  vec3(1.0f,0.0f,2.0f));
    T.setPositionKey(4.0f,  vec3(1.0f,0.0f,1.0f));

    T.setScaleKey(0.0f,  vec3(1.0f,0.0f,1.0f));
    T.setScaleKey(4.0f,  vec3(1.0f,5.0f,1.0f));

    quat Q;
    Q.w *= -1.0f;
    T.setRotKey(0.0f,  quat() );
    T.setRotKey(2.0f,  glm::rotate( quat(), 3.14159f/2.0f, vec3(0.0,1.0,0.0)) );
    T.setRotKey(4.0f,  quat());

    for(auto & x : T.mPKeys)
    {
        std::cout << "P: " << x.mValue.x  << "," << x.mValue.y << "," << x.mValue.z << std::endl;
    }

//    T.setScaleKey(0.0f, vec3(1.0f,1.0f,1.0f) );
//    T.setScaleKey(2.0f, vec3(2.0f,2.0f,2.0f) );



    Camera Cam;

    Cam.perspective(45, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT ,0.2f, 1000.0f);
    Cam.setPosition( vec3(10.0,5.0,10.0) );
    Cam.yaw( -45.0f* 3.14159/180.0f );
    Cam.pitch( 10.0f* 3.14159/180.0f );

    auto Axis = gla::createAxes().toGPU();

    // we can free the memory from the cpu since it is all on the gpu now.

    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram LineShader;
    LineShader.linkProgram(  VertexShader("shaders/Line_PC.v"),  FragmentShader("shaders/Line_PC.f")  );
    GLuint LineShaderCamMatrixId   = LineShader.getUniformLocation("inCameraMatrix");
    GLuint LineShaderModelMatrixID = LineShader.getUniformLocation("inModelMatrix");

    //==========================================================

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(gMainWindow) )
    {
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
        LineShader.useShader();
            LineShader.sendUniform_mat4(LineShaderCamMatrixId,   Cam.getProjectionMatrix() * Cam.getMatrix()   );
            LineShader.sendUniform_mat4(LineShaderModelMatrixID, glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
        Axis.Render();

       // std::cout << (float)fmod(Time.getElapsedTime(), 3.0)  << std::endl;

        LineShader.sendUniform_mat4(LineShaderModelMatrixID, T.getTransformationMatrix(  (float)fmod(Time.getElapsedTime(), 4.0) ) );
        Axis.Render();


        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    Axis.clear();
    LineShader.DeleteShader();

    glfwDestroyWindow(gMainWindow);
    glfwTerminate();
    return 0;
}



//=============================================================================
// Set up GLFW and GLEW
//=============================================================================
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow()
{
    glewExperimental = GL_TRUE;
    if (!glfwInit())
        exit(EXIT_FAILURE);

    auto gMainWindow = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

    if (!gMainWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(gMainWindow);

    int width, height;
    glfwGetFramebufferSize(gMainWindow, &width, &height);
    GLenum err = glewInit();

    return(gMainWindow);

}
//=============================================================================
