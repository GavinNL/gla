//#include <GL/glew.h>    // include GLEW and new version of GL on Windows

#include <stdio.h>

//#include <gla/global.h>
//#include <gla/camera.h>
//#include <gla/texture.h>
#include <gla/timer.h>
#include <gla/shader.h>
#include <gla/exper/sampler2darray.h>
#include <gla/exper/vertexarray.h>
#include <gla/exper/uniformbuffer.h>
#include <gla/exper/geometry.h>
#include <gla/transform.h>
#include <gla/camera.h>
#include <GLFW/glfw3.h> // GLFW helper library


using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "Uniform Buffers"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

struct Uniform140
{
    vec4 x;
};

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    { // create a scope around the main GL calls so that glfwTerminate is not called before
        // the gla objects are automatically destroyed.

        //===========================================================================
        // GLA code.
        //===========================================================================

        //---------------------------------------------------------------------------
        // Create two buffers on the CPU to hold position and colour information
        //---------------------------------------------------------------------------
        // Also create an index buffer.
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };

        std::vector< MyVertex > VertexBuffer;

        auto BoxVertices = gla::experimental::createBox();

     //   VertexBuffer.push_back( { vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f )} );
     //   VertexBuffer.push_back( { vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f )} );
     //   VertexBuffer.push_back( { vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f )} );
     //   VertexBuffer.push_back( { vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f )} );

        // Load teh buffer into the GPU
        gla::experimental::Array_Buffer buff( BoxVertices );

        gla::experimental::VertexArray VAO;
        VAO.Attach<glm::vec3, glm::vec2, glm::vec3>( buff );

        // Load some textures. And force using 3 components (r,g,b)
        gla::experimental::Image Tex1("../resources/textures/rocks.jpg",  3 );

        buff.Release();


        //=========================================================================================
        // Create the Texture array on the GPU.
        //   Note: Any objects that start with GPU mean they are initialized on the GPU
        //=========================================================================================
        gla::experimental::Sampler2D Samp1(Tex1);

        //===============================================================


        //---------------------------------------------------------------------------
        // Create a shader
        //---------------------------------------------------------------------------

        // Create the two shaders. The second argument is set to true because we are
        // compiling the shaders straight from a string. If we were compiling from a file
        // we'd just do:  VertexShader vs(Path_to_file);
        ShaderProgram TriangleShader;
        TriangleShader.AttachShaders(  VertexShader("../resources/shaders/Textures.v"),  FragmentShader("../resources/shaders/Textures.f")  );

        //==========================================================

        glEnable(GL_DEPTH_TEST);
        gla::Timer_T<float> Timer;

        gla::Transform T;

        T.SetEuler( { 0.3f, 4.,-2.2});

        gla::Camera C;
        C.SetPosition( {0.0,0.0,2.0f});
        C.Perspective(45.0, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1);

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            // Attach the Sampler to Texture Unit 0.
            Samp1.SetActive(0);

            T.SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * 0.4, -0.0});

            // Tell the shader that we are using Texture Unit 0 for the sampler
            TriangleShader.Uniform( TriangleShader.GetUniformLocation("uSampler"), 0 );
            TriangleShader.Uniform( TriangleShader.GetUniformLocation("uTransform"), C.GetProjectionMatrix() * C.GetMatrix() * T.GetMatrix() );


            // Draw the triangle.
            VAO.Draw(gla::experimental::Primitave::TRIANGLES, 36);


            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }

        // Clear the VAO
        // Since we had flagged the array buffers for deletion ,they will now be
        // cleared as well since they are no longer bound to any VAOs
        VAO.Release();

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

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return NULL;
    }


    return(gMainWindow);

}
//=============================================================================
