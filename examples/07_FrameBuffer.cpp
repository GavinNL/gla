/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>

#include "glad.h"

#include <gla/gla.h>
#include <gla/eng/atlas.h>
#include <gla/eng/mesh_buffer.h>

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

using namespace gla::experimental;

int main()
{


    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();


    { // create a scope around the main GL calls so that glfwTerminate is not called before
        // the gla objects are automatically destroyed.

        //===========================================================================
        // GLA code.
        //===========================================================================


        //====================== Create the geometry for the box ==============================
//#define BOX
#ifdef BOX
        auto BoxVertices = createBox();
        // Load the buffer into the GPU
        ArrayBuffer buff( BoxVertices );

        VertexArray VAO;
        VAO.Attach<glm::vec3, glm::vec2, glm::vec3>( buff );
#else
        auto SphereVertices = createCylinder(0.2f, 5);
        ArrayBuffer        Buff( SphereVertices.vertices );
        ElementArrayBuffer Ind( SphereVertices.indices );
        VertexArray VAO = VertexArray::MakeVAO<vec3,vec2,vec3>( Buff, Ind );
#endif
        // Load some textures. And force using 3 components (r,g,b)
        Image Tex1("./resources/textures/rocks.jpg",  3 );


        //====================== Create the Plane =============================================
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };
        std::vector< MyVertex > VertexBuffer;

        VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,0.0) }); // 3
        VertexBuffer.push_back( { glm::vec3(-1.0, -1.0, 0.0) , glm::vec2(0.0,1.0) }); // 0
        VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,1.0) }); // 1

        VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,0.0) }); // 0
        VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,1.0) }); // 2
        VertexBuffer.push_back( { glm::vec3( 1.0,  1.0, 0.0) , glm::vec2(1.0,0.0) }); // 2

        VertexArray  PlaneVAO;
        ArrayBuffer  PlaneBuff( VertexBuffer );

        PlaneVAO.Attach<glm::vec3, glm::vec2>(PlaneBuff);

        //=========================================================================================
        // Create the Texture array on the GPU.
        //   Note: Any objects that start with GPU mean they are initialized on the GPU
        //=========================================================================================
        Sampler2D Samp1(Tex1);

        //===============================================================


        //---------------------------------------------------------------------------
        // Create a shader
        //---------------------------------------------------------------------------

        // Create the two shaders. The second argument is set to true because we are
        // compiling the shaders straight from a string. If we were compiling from a file
        // we'd just do:  VertexShader vs(Path_to_file);
        ShaderProgram GBufferShader;
        GBufferShader.AttachShaders(  VertexShader(  "./resources/shaders/GBuffer.v"),
                                      FragmentShader("./resources/shaders/GBuffer.f")  );


        ShaderProgram GBufferSPass_Shader;
        GBufferSPass_Shader.AttachShaders(  VertexShader(  "./resources/shaders/GBuffer_SPass.v"),
                                            FragmentShader("./resources/shaders/GBuffer_SPass.f")  );

        //==========================================================

        glEnable(GL_DEPTH_TEST);
        Timer_T<float> Timer;
        Timer_T<float> Timer2;

        Transform T;

      //  T.SetEuler( { 0.3f, 4.,-2.2});
        T.SetPosition( {0,0,-3});
        Camera C;
        C.SetPosition( {0.0,0.0,0.0f});
        C.Perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f);

        FrameBuffer FBO;
        FBO.Generate();
        FBO.Bind();

        // Use framebuffer helper functions to create textures to be used for holding paricular types of data
        auto Positions = FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        auto Normals   = FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        auto Colours   = FrameBuffer::CreateBufferTexture_RGBA(     glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        auto Depth     = FrameBuffer::CreateBufferTexture_Depth16F( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );

        //FBO.Bind();
        FBO.Attach(Positions, FrameBufferAttachment::COLOR0);
        FBO.Attach(Normals,   FrameBufferAttachment::COLOR1);
        FBO.Attach(Colours,   FrameBufferAttachment::COLOR2);
        FBO.Attach(Depth  ,   FrameBufferAttachment::DEPTH);

        FBO.Use( {FrameBufferAttachment::COLOR0 , FrameBufferAttachment::COLOR1, FrameBufferAttachment::COLOR2} );

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
#ifdef BOX
            VAO.Draw(Primitave::TRIANGLES, 36 );
#else
            VAO.Draw(Primitave::TRIANGLES, SphereVertices.indices.size() );
            //VAO.DrawInstanced(Primitave::TRIANGLES, SphereVertices.indices.size(), 3 );
#endif

            FBO.UnBind();

            GBufferSPass_Shader.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Positions.SetActive(0);
            Normals.SetActive(1);
            Colours.SetActive(2);
            Depth.SetActive(3);

            static int i=0;
            if( Timer2.getElapsedTime() > 2.0 )
            {
                i = (i+1)%4;
                Timer2.reset();
            }

            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gPosition")  , 0 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gNormal")    , 1 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gAlbedoSpec"), 2 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gDepth")     , 3 );

            PlaneVAO.Draw(Primitave::TRIANGLES, 6 );

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
