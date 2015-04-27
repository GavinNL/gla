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

    // Also create an index buffer.
    u3ArrayBuffer cpuIndex;

    cpuPositions.insert( vec3(-1.0f, -1.0f, 0.f));
    cpuPositions.insert( vec3( 1.0f ,-1.0f, 0.f));
    cpuPositions.insert( vec3( 0.0f , 1.0f, 0.f));

    cpuColours.insert( vec4(1.f, 0.f, 0.f, 1.0f) );
    cpuColours.insert( vec4(0.f, 1.f, 0.f, 1.0f) );
    cpuColours.insert( vec4(0.f, 0.f, 1.f, 1.0f) );

    cpuIndex.insert( uvec3(0,1,2) );

    // Copy the CPU buffers to the GPU.
    GPUArrayBuffer gpuPositions = cpuPositions.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum
    GPUArrayBuffer gpuColours   =   cpuColours.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum
    GPUArrayBuffer gpuIndex     =    cpuIndex.toGPU(ELEMENT_ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum

    // The data in the CPU buffers are no longer needed. We can clear their memory
    cpuPositions.clear();
    cpuColours.clear();
    cpuIndex.clear();

    // Create a VertexArrayObject with Positions as the first attribute, colours as the second attribute
    // Use the gpuIndex as the index buffer
    // and use TRIANGLES as the render method.
    GPUArrayObject VAO(  TRIANGLES,
                        {gpuPositions, gpuColours},
                         gpuIndex);


    /* NOTE: We can also create a VAO without a Index buffer, simple call
     * GPUArrayObject VAO( TRIANGLES, {gpuPositions, gpuColours}); */


    // GPUArrayBuffers are bound to the VAO now, we can clear the buffers, but they wont
    // be removed from GPU memory until we clear the VOA
    gpuPositions.clear();
    gpuColours.clear();
    gpuIndex.clear();

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
