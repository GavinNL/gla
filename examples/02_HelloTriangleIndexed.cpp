#include <gla/glad.h>
#include <gla/exper/array_buffer.h>
#include <gla/exper/sampler2d.h>
#include <gla/shader.h>
#include <GLFW/glfw3.h>
#include <gla/exper/sampler2darray.h>
//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Hello Indexed Triangle"
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
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec4 c;
        };

        std::vector< MyVertex > VertexBuffer;
        VertexBuffer.push_back( { glm::vec3(-1.0f, -1.0f, 0.f), glm::vec4(1.f, 0.f, 0.f, 1.0f)  }  );
        VertexBuffer.push_back( { glm::vec3( 1.0f ,-1.0f, 0.f), glm::vec4(0.f, 1.f, 0.f, 1.0f)  }  );
        VertexBuffer.push_back( { glm::vec3( 0.0f , 1.0f, 0.f), glm::vec4(0.f, 0.f, 1.f, 1.0f)  }  );

        // Create a buffer on the GPU using the data from the standard vector
        gla::experimental::Array_Buffer         G( VertexBuffer );
        //================================================================



        //================================================================
        // 2. Create an index buffer that will indicate which vertices
        //    in the Array_Buffer will make up the triangles
        //================================================================
        // Create a triangle from vertex 0 1 and 2
        std::vector< glm::uvec3 > IndexBuffer;
        IndexBuffer.push_back( glm::uvec3( 0 ,1, 2) );

        // Send the index buffer to the gpu.
        gla::experimental::Element_Array_Buffer E( IndexBuffer );
        //================================================================


        while (!glfwWindowShouldClose(gMainWindow) )
        {
            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            // Tell OpenGL that the data in the buffer
            // consists of one vec3 and one vec4 that are both un-normalized.
            // This function will automatically bind the array buffer and set the
            // attributes.
            G.EnableAttributes<glm::vec3, glm::vec4>( {false,false } );
            // (alternatively) G.EnableAttributes<glm::vec3, glm::vec4>(); // same as non-normlaized vectors



            // Draw the Index Buffer, using every 3 indices to form a triangle.
            // G will automatically be bound when calling EnableAttributes
            E.Draw(gla::experimental::Primitave::TRIANGLES, 3);

            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }
    }


    glfwDestroyWindow(gMainWindow);
    glfwTerminate();

    return 0;
}

#if 0
//#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

//#include <gla/global.h>
//#include <gla/camera.h>
//
//#include <gla/timer.h>
//#include <gla/shader.h>
//
//#include <gla/arraybuffer.h>
#include <locale>
#include <gla/exper/array_buffer.h>

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

    /* NOTE:
     *    All objects on the GPU (GPUArrayBuffers, GPUArrayObjects, GPUTexture, GPUTextureArray are copyable assignable.
     * They are also self-destroying, so they will unallocate any memory on the GPU when they go out of scope. Memory
     * will only be freed from the GPU once all objects have gone out of scope.
     *
     */

    struct MyVertex
    {
        glm::vec3 p;
        glm::vec4 c;
    };

    std::vector< MyVertex > CpuBuffer;

    CpuBuffer.push_back( { vec3(-1.0f, -1.0f, 0.f), vec4(1.f, 0.f, 0.f, 1.0f)  }  );
    CpuBuffer.push_back( { vec3( 1.0f ,-1.0f, 0.f), vec4(0.f, 1.f, 0.f, 1.0f)  }  );
    CpuBuffer.push_back( { vec3( 0.0f , 1.0f, 0.f), vec4(0.f, 0.f, 1.f, 1.0f)  }  );

    gla::experimental::Array_Buffer G( CpuBuffer );

    G.EnableAttributes<glm::vec3, glm::vec4>( {false,false } );
    //auto G = Buf.ToGPU();

    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    TriangleShader.AttachShaders(  VertexShader("shaders/HelloTriangle.v"),  FragmentShader("shaders/HelloTriangle.f")  );


    //gla::EnableVertexAttribute<vec3, vec4>( G );
    //==========================================================



    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Set the triangle shader to be the one that we will use
        TriangleShader.Bind();

        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
        //G.Render(Primitave::TRIANGLES, 0, 3);

        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }


    glfwDestroyWindow(gMainWindow);
    glfwTerminate();
    return 0;
}



#endif
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

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return NULL;
    }

    return(gMainWindow);

}
//=============================================================================

