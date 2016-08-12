//#include <GL/glew.h>    // include GLEW and new version of GL on Windows

#include <stdio.h>

#include "glad.h"
//#include <gla/global.h>
//#include <gla/camera.h>
//#include <gla/texture.h>
#include <gla/timer.h>
#include <gla/exper/shader.h>
#include <gla/exper/sampler2darray.h>
#include <gla/exper/vertexarray.h>
#include <gla/exper/uniformbuffer.h>
#include <gla/exper/geometry.h>
#include <gla/exper/framebuffer.h>
#include <gla/transform.h>
#include <gla/camera.h>
#include <GLFW/glfw3.h> // GLFW helper library


using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "Framebuffers and Differed Rendering"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

int main()
{

    namespace GLA = gla::experimental;

    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();


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

        auto BoxVertices = GLA::createBox();

        // Load teh buffer into the GPU
        GLA::Array_Buffer buff( BoxVertices );

        GLA::VertexArray VAO;
        VAO.Attach<glm::vec3, glm::vec2, glm::vec3>( buff );

        // Load some textures. And force using 3 components (r,g,b)
        GLA::Image Tex1("../resources/textures/rocks.jpg",  3 );

        buff.Release();

        VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,0.0) }); // 3
        VertexBuffer.push_back( { glm::vec3(-1.0, -1.0, 0.0) , glm::vec2(0.0,1.0) }); // 0
        VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,1.0) }); // 1

        VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,0.0) }); // 0
        VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,1.0) }); // 2
        VertexBuffer.push_back( { glm::vec3( 1.0,  1.0, 0.0) , glm::vec2(1.0,0.0) }); // 2


        GLA::VertexArray  PlaneVAO;
        GLA::Array_Buffer PlaneBuff( VertexBuffer );
        PlaneVAO.Attach<glm::vec3, glm::vec2>(PlaneBuff);

        //=========================================================================================
        // Create the Texture array on the GPU.
        //   Note: Any objects that start with GPU mean they are initialized on the GPU
        //=========================================================================================
        GLA::Sampler2D Samp1(Tex1);

        //===============================================================


        //---------------------------------------------------------------------------
        // Create a shader
        //---------------------------------------------------------------------------

        // Create the two shaders. The second argument is set to true because we are
        // compiling the shaders straight from a string. If we were compiling from a file
        // we'd just do:  VertexShader vs(Path_to_file);
        GLA::ShaderProgram GBufferShader;
        GBufferShader.AttachShaders(  GLA::VertexShader("../resources/shaders/GBuffer.v"),
                                      GLA::FragmentShader("../resources/shaders/GBuffer.f")  );


        GLA::ShaderProgram GBufferSPass_Shader;
        GBufferSPass_Shader.AttachShaders(  GLA::VertexShader("../resources/shaders/GBuffer_SPass.v"),
                                            GLA::FragmentShader("../resources/shaders/GBuffer_SPass.f")  );

        //==========================================================

        glEnable(GL_DEPTH_TEST);
        gla::Timer_T<float> Timer;

        gla::Transform T;

      //  T.SetEuler( { 0.3f, 4.,-2.2});
        T.SetPosition( {0,0,-3});
        gla::Camera C;
        C.SetPosition( {0.0,0.0,0.0f});
        C.Perspective(45.0, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1);

        GLA::FrameBuffer FBO;
        FBO.Generate();
        FBO.Bind();

        auto Positions = GLA::FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        auto Normals   = GLA::FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        auto Colours   = GLA::FrameBuffer::CreateBufferTexture_RGBA(     glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        auto Depth     = GLA::FrameBuffer::CreateBufferTexture_Depth16F( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );

        //FBO.Bind();
        FBO.Attach(Positions, GLA::FrameBufferAttachment::COLOR0);
        FBO.Attach(Normals,   GLA::FrameBufferAttachment::COLOR1);
        FBO.Attach(Colours,   GLA::FrameBufferAttachment::COLOR2);
        FBO.Attach(Depth  ,   GLA::FrameBufferAttachment::DEPTH);

        FBO.Use( {GLA::FrameBufferAttachment::COLOR0 , GLA::FrameBufferAttachment::COLOR1, GLA::FrameBufferAttachment::COLOR2} );

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR!!!" << std::endl;



        while (!glfwWindowShouldClose(gMainWindow) )
        {

            FBO.Bind();
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Set the triangle shader to be the one that we will use
            GBufferShader.Bind();

            // Attach the Sampler to Texture Unit 0.
            Samp1.SetActive(0);

            T.SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * 0.4, -0.0 } );

            // Tell the shader that we are using Texture Unit 0 for the sampler
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uSampler"), 0 );
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uTransform"),  T.GetMatrix() );
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uCamera"),  C.GetProjectionMatrix() );

            // Draw the triangle.
            VAO.Draw(GLA::Primitave::TRIANGLES, 36);

            FBO.UnBind();

            GBufferSPass_Shader.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Positions.SetActive(0);
            Normals.SetActive(1);
            Colours.SetActive(2);
            Depth.SetActive(3);

            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gPosition")  , 0 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gNormal")    , 1 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gAlbedoSpec"), 2 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gDepth")     , 3 );

            PlaneVAO.Draw(GLA::Primitave::TRIANGLES, 6);

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
