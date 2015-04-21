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

    // A cpuVAO holds cpuArraybuffers. It's a convenient class to help
    // create a VAO
    VertexArrayObject cpuVAO;

    // Create two buffers, they will return the position
    // of the buffer.
    int positionIndex = cpuVAO.createBuffer<vec3>();  // this will be 0
    int colourIndex   = cpuVAO.createBuffer<vec4>();  // this will be 1

    // insert the positions and colours
    cpuVAO.insert(positionIndex, vec3(-1.0f, -1.0f, 0.f) );
    cpuVAO.insert(positionIndex, vec3( 1.0f ,-1.0f, 0.f) );
    cpuVAO.insert(positionIndex, vec3( 0.0f , 1.0f, 0.f) );

    // attempting to insert a vec4 into a vec3 buffer will not do anything
    cpuVAO.insert(positionIndex, vec4( 0.0f , 1.0f, 0.f, 1.0) );

    cpuVAO.insert(colourIndex,   vec4(1.f, 0.f, 0.f, 1.0f) );
    cpuVAO.insert(colourIndex,   vec4(0.f, 1.f, 0.f, 1.0f) );
    cpuVAO.insert(colourIndex,   vec4(0.f, 0.f, 1.f, 1.0f) );

    //========================= Optional =====================================
    // you can commment this out
    //========================================================================

    //========================================================================

    // send all the buffers to the GPU and  return a GPUArrayObject
    // NOTE: All Arrays in the VAO must be at least as large as the first buffer.
    GPUArrayObject VAO = cpuVAO.toGPU();

    // we can free the memory from the cpu since it is all on the gpu now.

    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    TriangleShader.linkProgram(  VertexShader("shaders/HelloTriangle.v"),  FragmentShader("shaders/HelloTriangle.f")  );


    //==========================================================

    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
        VAO.bind();
        VAO.Render(TRIANGLES);

        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    VAO.clear();


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
