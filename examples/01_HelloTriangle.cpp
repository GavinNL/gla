
#include "glad.h"
#include <gla/exper/buffers.h>
#include <gla/exper/sampler2d.h>
#include <gla/shader.h>
#include <GLFW/glfw3.h>

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


    { // adding an extra scope here because we want all gla objects automatically destroyed when they go out of scope
      // calling glfwTerminate before destroying the openGL objects will cause a segfault, so putting
      // this scope here will make the gla objects destroy themselves before terminate gets called.

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);



        gla::ShaderProgram TriangleShader;
        TriangleShader.AttachShaders(  gla::VertexShader("../resources/shaders/HelloTriangle.v"),
                                       gla::FragmentShader("../resources/shaders/HelloTriangle.f")  );


        //================================================================
        // 1. Create the vertices of the triangle using our vertex structure
        //    The vertex strucutre contains positions and colours of each
        //    vertex in the triangle.
        //================================================================
        struct MyVertex   // You don't actually need to create a struct for the vertex
        {                 // as long as you keep the positions and colour values
            glm::vec3 p;  // in their proper order...eg:
            glm::vec4 c;  // p1,c1,p2,c2,p3,c3...etc
        };                //

        // Populate a standard vector of the vertices with the appropriate information
        std::vector< MyVertex > CpuBuffer;
        CpuBuffer.push_back( { glm::vec3(-1.0f, -1.0f, 0.f), glm::vec4(1.f, 0.f, 0.f, 1.0f)  }  );
        CpuBuffer.push_back( { glm::vec3( 1.0f ,-1.0f, 0.f), glm::vec4(0.f, 1.f, 0.f, 1.0f)  }  );
        CpuBuffer.push_back( { glm::vec3( 0.0f , 1.0f, 0.f), glm::vec4(0.f, 0.f, 1.f, 1.0f)  }  );

        // Create a buffer on the GPU using the data from the standard vector
        gla::experimental::Array_Buffer         G;

        G << CpuBuffer;


        gla::experimental::Array_Buffer g;
        g = G;

        G.Release();
        //====


        while (!glfwWindowShouldClose(gMainWindow) )
        {

            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();


            // Tell OpenGL that the data in the buffer
            // consists of one vec3 and one vec4 that are both un-normalized.
            // This function will automatically bind the array buffer and set the
            // attributes.
            g.EnableAttributes<glm::vec3, glm::vec4>( {false,false } );
            // (alternatively) G.EnableAttributes<glm::vec3, glm::vec4>(); // same as non-normlaized vectors


            // Now draw the triangle.
            // we are drawing Triangles, starting at Vertex Index 0
            // and we are drawing 3 vertices (because 3 vertices make a triangle)
            g.Draw(gla::experimental::Primitave::TRIANGLES, 0, 3);


            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }
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
    //    glewExperimental = GL_TRUE;

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
    //    GLenum err = glewInit();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return NULL;
    }

    return(gMainWindow);

}
//=============================================================================

