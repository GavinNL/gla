//#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>


#include <gla/global.h>
#include <gla/camera.h>
//#include <gla/texture.h>
#include <gla/timer.h>
#include <gla/shader.h>
//#include <gla/gputexturearray.h>
#include <gla/vertexarrayobject.h>
//#include <gla/framebufferobject.h>
//#include <gla/solids.h>
//#include <gla/uniformbuffer.h>
//#include <gla/arraybuffer.h>
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


    InterleavedVAO<vec3, vec4> tri;

    tri.InsertVertex( vec3(-1.0f, -1.0f, 0.f), vec4(1.f, 0.f, 0.f, 1.0f) );
    tri.InsertVertex( vec3( 1.0f ,-1.0f, 0.f), vec4(0.f, 1.f, 0.f, 1.0f) );
    tri.InsertVertex( vec3( 0.0f , 1.0f, 0.f), vec4(0.f, 0.f, 1.f, 1.0f) );

    tri.InsertElement( uvec2{0,1} );
    tri.InsertElement( uvec2{1,2} );
    tri.InsertElement( uvec2{2,0} );

    auto VAO = tri.ToGPU();

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    GLA_DOUT  << "attaching shaders: " << std::endl;
    TriangleShader.AttachShaders(  VertexShader("shaders/HelloTriangle.v"),  FragmentShader("shaders/HelloTriangle.f")  );


    VAO = tri.ToGPU();
    //==========================================================
    GLA_DOUT  << "Starting" << std::endl;
    while (!glfwWindowShouldClose(gMainWindow) )
    {

        //auto VAO2 = VAO;
        //GLA_DOUT  << VAO2._size << std::endl;
        // Set the triangle shader to be the one that we will use
        TriangleShader.Bind();

        // Bind the VAO so it can be used next.
       // VAO2.Bind();

        // renders the VAO as triangles (assume triangles since our Index buffer is uvec3, if it was uvec4, it would assume quads
        VAO.Render();

        // To specify exactly what primitave you want. Use the following.
        //  VAO.Render(TRIANGLES);  // Can also use this.

        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    //VAO.clear();


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
