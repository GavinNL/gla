#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>


#include <gla/global.h>
#include <gla/camera.h>
//#include <gla/texture.h>
#include <gla/timer.h>
#include <gla/shader.h>
//#include <gla/gputexturearray.h>
//#include <gla/vertexarrayobject.h>
//#include <gla/framebufferobject.h>
//#include <gla/solids.h>
//#include <gla/uniformbuffer.h>
#include <gla/arraybuffer.h>
#include <locale>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================
struct  Test
{
    GPUArrayBuffer<ArrayType::ARRAY_BUFFER> A;
};

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    //===========================================================================
    // GLA code.
    //===========================================================================

    /* NOTE:
     *    All objects on the GPU (GPUArrayBuffers, GPUArrayObjects, GPUTexture, GPUTextureArray are copyable assignable.
     * They are also self-destroying, so they will unallocate any memory on the GPU when they go out of scope. Memory
     * will only be freed from the GPU once all objects have gone out of scope.
     *
     */


    using MyVertex       = std::tuple<vec3, vec4>;
    using MyVertexBuffer = gla::ArrayBuffer< MyVertex >;
    MyVertexBuffer Buf;


    Buf.Insert(  std::make_tuple(vec3(-1.0f, -1.0f, 0.f), vec4(1.f, 0.f, 0.f, 1.0f)  )  );
    Buf.Insert(  std::make_tuple(vec3( 1.0f ,-1.0f, 0.f), vec4(0.f, 1.f, 0.f, 1.0f)  )  );
    Buf.Insert(  std::make_tuple(vec3( 0.0f , 1.0f, 0.f), vec4(0.f, 0.f, 1.f, 1.0f)  )  );

    //auto G = Buf.toGPU(ARRAY_BUFFER);
    auto G = Buf.ToGPU();

    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    TriangleShader.AttachShaders(  VertexShader("shaders/HelloTriangle.v"),  FragmentShader("shaders/HelloTriangle.f")  );


    gla::EnableVertexAttribute<vec3, vec4>(G);
    //==========================================================



    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Set the triangle shader to be the one that we will use
        TriangleShader.Bind();

        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
        G.Render(Primitave::TRIANGLES, 0, 3);

        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }


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

