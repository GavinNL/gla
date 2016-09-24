#include "glad.h"
#include <gla/exper/array_buffer.h>
#include <gla/exper/sampler2d.h>
#include <gla/exper/shader.h>
#include <gla/exper/sampler2darray.h>
#include <gla/exper/vertexarray.h>
#include <GLFW/glfw3.h>
//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "HelloTriangleVertexArray"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================


using namespace gla::experimental;

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }



    { // adding an extra scope here because we want all gla objects automatically destroyed when they go out of scope
      // calling glfwTerminate before destroying the openGL objects will cause a segfault, so putting
      // this scope here will make the gla objects destroy themselves before terminate gets called.

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);



        ShaderProgram TriangleShader;
        TriangleShader.AttachShaders(  VertexShader("../resources/shaders/HelloTriangle.v"),  FragmentShader("../resources/shaders/HelloTriangle.f")  );


        //====
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec4 c;
        };

        std::vector< MyVertex > VertexBuffer;
        VertexBuffer.push_back( { glm::vec3(-1.0f, -1.0f, 0.f), glm::vec4(1.f, 0.f, 0.f, 1.0f)  }  );
        VertexBuffer.push_back( { glm::vec3( 1.0f ,-1.0f, 0.f), glm::vec4(0.f, 1.f, 0.f, 1.0f)  }  );
        VertexBuffer.push_back( { glm::vec3( 0.0f , 1.0f, 0.f), glm::vec4(0.f, 0.f, 1.f, 1.0f)  }  );


        // Create a triangle from vertex 0 1 and 2
        std::vector< glm::uvec3 > IndexBuffer;
        IndexBuffer.push_back( glm::uvec3( 0 ,1, 2) );


        // Send the vertex buffer to the GPU
        ArrayBuffer         G( VertexBuffer );
        ElementArrayBuffer  E( IndexBuffer );


        // Create a vertex array object.
        VertexArray VAO;

        VAO.Attach<glm::vec3, glm::vec4>( E, G );


        G.Unbind();
        E.Unbind();
        //====


        while (!glfwWindowShouldClose(gMainWindow) )
        {

            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            // Bind the VertexBuffer and tell openGL that
            VAO.Draw( Primitave::TRIANGLES, 3 );

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

    auto gMainWindow = glfwCreateWindow(640, 480, WINDOW_TITLE, NULL, NULL);

    if (!gMainWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(gMainWindow);

    int width, height;
    glfwGetFramebufferSize(gMainWindow, &width, &height);
    //    GLenum err = glewInit();

    return(gMainWindow);

}
//=============================================================================

