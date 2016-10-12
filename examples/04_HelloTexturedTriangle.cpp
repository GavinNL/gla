#include "glad.h"
#include <gla/gla.h>
//#include <gla/exper/sampler2d.h>
//#include <gla/exper/buffers.h>
//#include <gla/exper/shader.h>

#include <glm/gtc/noise.hpp>
#include <GLFW/glfw3.h>

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Hello Textured Triangle"
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


        //================================================================
        // 1. Create the vertices of the triangle using our vertex structure
        //    The vertex strucutre contains positions and UV coords of each
        //    vertex in the triangle.
        //================================================================
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };

        std::vector< MyVertex >     VertexData;
        std::vector< unsigned int > IndexData;
        VertexData.push_back( { glm::vec3(-1.0f, -1.0f, 0.f), glm::vec2(0.f, 1.f)  }  );
        VertexData.push_back( { glm::vec3( 1.0f ,-1.0f, 0.f), glm::vec2(1.f, 1.f)  }  );
        VertexData.push_back( { glm::vec3( 0.0f , 1.0f, 0.f), glm::vec2(.5f, .0f)  }  );

        IndexData.push_back(0);
        IndexData.push_back(1);
        IndexData.push_back(2);
        // Send the vertex data to the GPU.
        ArrayBuffer        G( VertexData );
        ElementArrayBuffer E( IndexData );

        // Create a VertexArray from the data
        VertexArray VAO = VertexArray::MakeVAO<glm::vec3, glm::vec2>( G, E);

        //================================================================


        //================================================================
        // 2. Load an image we want to use as the texture
        //================================================================

        Image Img;

        Img.loadFromPath("./resources/textures/rocks1024.jpg" );

        Sampler2D Sampler(Img);

        // We can modify the red channel using a lambda function
        Img.r = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*8.0f ) + 0.5);  };

        // Or similarly using the macro which essentially does the same thing as the above
        // but reduces the amount you need to write
        Img.b = IMAGE_EXPRESSION( glm::clamp(2*x + y,0.0f,1.0f) );


        // A texture in GLSL is called a Sampler2D, we send the data to the GPU
        // by creating a Sampler2D object and initializing it with the Image object
        Sampler.PasteSubImage( uvec2(0,0), Img);


        //================================================================
        // 3. Load the shader we want to use when drawsing the triangle
        //         Can use one or the other.
        //  The .s shader file has each component of the shader
        //  separated by xml tags:  <vertex>, <fragment> <geometry> <tessellation_control>
        //  and <tessellation_evaluation>
        //================================================================

#if 0
        ShaderProgram TriangleShader;
        TriangleShader.AttachShaders(  VertexShader(  "../resources/shaders/Textures.v"),
                                       FragmentShader("../resources/shaders/Textures.f"));
#else
        //auto TriangleShader = ShaderProgram::Load("../resources/shaders/Textures.s");
        auto TriangleShader = ShaderProgram::Load("./resources/shaders/Textures.s");
#endif

        //================================================================

        while ( !glfwWindowShouldClose(gMainWindow) )
        {
            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            // Attach the Sampler to Texture Unit 0.
            Sampler.SetActive(0);

            // Tell the shader that we are using Texture Unit 0 for the sampler
            TriangleShader.Uniform( TriangleShader.GetUniformLocation("uSampler")  , 0           );
            TriangleShader.Uniform( TriangleShader.GetUniformLocation("uTransform"), glm::mat4() );

            // Draw the triangle.
            VAO.Draw(Primitave::TRIANGLES, 3);

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

    return(gMainWindow);

}
//=============================================================================

