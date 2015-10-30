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
    v3ArrayBuffer cpuPos;
    v4ArrayBuffer cpuCol;


    //---------------------------------------------------------------------------
    // Populate the buffers with the appropriate data.
    //---------------------------------------------------------------------------
    cpuPos.insert( vec3(-1.0f, -1.0f, 0.f));
    cpuPos.insert( vec3( 1.0f ,-1.0f, 0.f));
    cpuPos.insert( vec3( 0.0f , 1.0f, 0.f));

    cpuCol.insert( vec4(1.f, 0.f, 0.f, 1.0f) );
    cpuCol.insert( vec4(0.f, 1.f, 0.f, 1.0f) );
    cpuCol.insert( vec4(0.f, 0.f, 1.f, 1.0f) );

    //---------------------------------------------------------------------------
    // Copy the CPU buffers to the GPU.
    //---------------------------------------------------------------------------
    //GPUArrayBuffer gpuPos   = cpuPos.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum
    //GPUArrayBuffer gpuCol   = cpuCol.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum


    /* NOTE:
     *    All objects on the GPU (GPUArrayBuffers, GPUArrayObjects, GPUTexture, GPUTextureArray are copyable assignable.
     * They are also self-destroying, so they will unallocate any memory on the GPU when they go out of scope. Memory
     * will only be freed from the GPU once all objects have gone out of scope.
     *
     */


    // The data in the CPU buffers are no longer needed. We can clear their memory
    //cpuPos.clear();
    //cpuCol.clear();

    using MyVertex       = gla::MemoryAlignedTuple<vec3, vec4>;
    using MyVertexBuffer = gla::ArrayBuffer_T< MyVertex >;
    MyVertexBuffer Buf;


    Buf.insert(  MyVertex(vec3(-1.0f, -1.0f, 0.f), vec4(1.f, 0.f, 0.f, 1.0f) ) );
    Buf.insert(  MyVertex(vec3( 1.0f ,-1.0f, 0.f), vec4(0.f, 1.f, 0.f, 1.0f) ) );
    Buf.insert(  MyVertex(vec3( 0.0f , 1.0f, 0.f), vec4(0.f, 0.f, 1.f, 1.0f) ) );

    auto G = Buf.toGPU(ARRAY_BUFFER);


    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    TriangleShader.linkProgram(  VertexShader("shaders/HelloTriangle.v"),  FragmentShader("shaders/HelloTriangle.f")  );


    G.bind(ARRAY_BUFFER);
    gla::EnableVertexAttribArray<vec3, vec4>();
    //==========================================================

    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Set the triangle shader to be the one that we will use
        TriangleShader.useShader();

        // Enable the two buffers as attributes 0 and 1 and draw the arrays
        // The buffers will automatically bind.
        //gpuPos.EnableAttribute(0);
        //gpuCol.EnableAttribute(1);

        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
//        gpuPos.Render(TRIANGLES);
        G.Render(TRIANGLES);
        // or: gpuColours.Render(TRIANGLES);
        // or  gpuColours.Render(TRIANGLES, 3);
        // or  gpuColours.Render(TRIANGLES, 0, 3);
        // or: glDrawArrays( GL_TRIANGLES,  0, 3);

        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }




//    {
//        auto x = gpuPos;
//        auto y = gpuCol;
//
//        // Clearing the two buffers will not do anything because x and y
//        // still exist. Once x and y go out of scope, then only the memory will be freed
//        gpuPos.clear();
//        gpuCol.clear();
//    }

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

