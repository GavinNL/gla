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

    //---------------------------------------------------------------------------
    // Create two buffers on the CPU to hold position and colour information
    //---------------------------------------------------------------------------
    v3ArrayBuffer cpuPositions;
    v4ArrayBuffer cpuColours;


    //---------------------------------------------------------------------------
    // Populate the buffers with the appropriate data.
    //---------------------------------------------------------------------------
    cpuPositions.insert( vec3(-1.0f, -1.0f, 0.f));
    cpuPositions.insert( vec3( 1.0f ,-1.0f, 0.f));
    cpuPositions.insert( vec3( 0.0f , 1.0f, 0.f));

    cpuColours.insert( vec4(1.f, 0.f, 0.f, 1.0f) );
    cpuColours.insert( vec4(0.f, 1.f, 0.f, 1.0f) );
    cpuColours.insert( vec4(0.f, 0.f, 1.f, 1.0f) );

    //---------------------------------------------------------------------------
    // Copy the CPU buffers to the GPU.
    //---------------------------------------------------------------------------
    GPUArrayBuffer gpuPositions = cpuPositions.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum
    GPUArrayBuffer gpuColours   =   cpuColours.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum
    /* NOTE:
     *    All objects on the GPU (GPUArrayBuffers, GPUArrayObjects, GPUTexture, GPUTextureArray are copyable assignable.
     * These objects do not automatically free the GPU data when their destructor is called. YOU must explicitally call
     * GPUArrayBuffer.clear(). If you have copied a buffer, by GPUBufferB = GPUBufferA, then clearing GPUBufferA,
     * will also clear GPUBufferB.
     */


    // The data in the CPU buffers are no longer needed. We can clear their memory
    cpuPositions.clear();
    cpuColours.clear();

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

        // Enable the two buffers as attributes 0 and 1 and draw the arrays
        // The buffers will automatically bind.
        gpuPositions.EnableAttribute(0);
        gpuColours   .EnableAttribute(1);

        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
        gpuColours.Render(TRIANGLES);
        // or: gpuColours.Render(TRIANGLES);
        // or  gpuColours.Render(TRIANGLES, 3);
        // or  gpuColours.Render(TRIANGLES, 0, 3);
        // or: glDrawArrays( GL_TRIANGLES,  0, 3);

        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // We should probably clear the memory from the GPU to be safe.
    gpuPositions.clear();
    gpuColours.clear();


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
