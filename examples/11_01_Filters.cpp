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

#define GLA_LOG_ALL

#include <stdio.h>

#include "glad.h"

#include <gla/gla.h>
#include <gla/eng/atlas.h>
#include <gla/eng/meshbuffer.h>
#include <gla/eng/rendertotexture.h>
#include <gla/eng/sampler2dfilter.h>
#include <GLFW/glfw3.h> // GLFW helper library



/**
 * This is the same as 07_FrameBuffers, but uses the
 * RenderToTexture helper class to build the framebuffers.
 */


using namespace gla;

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define WINDOW_TITLE  "Framebuffers and Differed Rendering"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

using namespace gla;

int main()
{


    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();


    { // create a scope around the main GL calls so that glfwTerminate is not called before
        // the gla objects are automatically destroyed.

        //===========================================================================
        // GLA code.
        //===========================================================================

        //================================================================
        // 1. Create the plane to use during the second render pass
        //================================================================
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };
        std::vector< MyVertex > VertexBuffer;

        VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,1.0) }); // 3
        VertexBuffer.push_back( { glm::vec3(-1.0, -1.0, 0.0) , glm::vec2(0.0,0.0) }); // 0
        VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,0.0) }); // 1

        VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,1.0) }); // 0
        VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,0.0) }); // 2
        VertexBuffer.push_back( { glm::vec3( 1.0,  1.0, 0.0) , glm::vec2(1.0,1.0) }); // 2

        VertexArray  PlaneVAO;
        ArrayBuffer  PlaneBuff( VertexBuffer );

        PlaneVAO.Attach<glm::vec3, glm::vec2>(PlaneBuff);


        //================================================================
        // 2. Load the two shaders we are going to use
        //       - The G Buffer Shader used to draw geometry
        //       - The Shader pass
        //================================================================

        ShaderProgram FilterPassShader = ShaderProgram::Load( "./resources/shaders/FilterPass.s" );


        //==========================================================




        //================================================================
        // 3  Create a Render To Texture object
        //
        //================================================================

        // Load some textures. And force using 3 components (r,g,b)
        Image Tex1("./resources/textures/rocks.jpg",  3 );
        // send the image to the GPU
        Sampler RocksTexture(Tex1);


        // Create two blank RGB textures that will hold
        // the newly flitered texture
        auto Sampler1 = Sampler::RGBTexture(Tex1.size());
        auto Sampler2 = Sampler::RGBTexture(Tex1.size());

        // Create a rendertotexture object
        RenderToTexture FilterRTT;

        // attach Sampler1 to RTT so tht anything drawn will draw to the sampler
        FilterRTT.Attach(Sampler1);   // default second paramter it to COLOR0
        FilterRTT.Bind();

        // disable depth dest because we do not need it
        glDisable(GL_DEPTH_TEST);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glViewport(0, 0, Tex1.size().x, Tex1.size().y);


        // Use the shader, send all the uniforms over
        // and draw a fullscreen quad of the input texture
        FilterPassShader.Bind();

        RocksTexture.SetActive(0); // use the rocks as the input texture
        FilterPassShader.Uniform( FilterPassShader.GetUniformLocation("u_Texture"), 0);
        FilterPassShader.Uniform( FilterPassShader.GetUniformLocation("u_Direction"), vec2(0.0, 2.0) );
        FilterPassShader.Uniform( FilterPassShader.GetUniformLocation("u_TexelSize"), 1.0f/vec2( Tex1.size() ));

        PlaneVAO.Draw(Primitave::TRIANGLES, 6 );


        Sampler2.SetFilter( Sampler::LINEAR, Sampler::LINEAR);
        Sampler1.SetFilter( Sampler::LINEAR, Sampler::LINEAR);



        // Attach the second sampler
        FilterRTT.Attach(Sampler2);   // default second paramter it to COLOR0
        // and use the first sampler as the input texture
        Sampler1.SetActive(0);
        FilterRTT.Bind();

        // filter the image again using different uniform parameters
        FilterPassShader.Uniform( FilterPassShader.GetUniformLocation("u_Direction"), vec2(2.0, 0.0) );
        PlaneVAO.Draw(Primitave::TRIANGLES, 6 );
        FilterRTT.Unbind();
        glEnable(GL_DEPTH_TEST);


        Timer_T<float> Timer;

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

            glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );
            FilterPassShader.Bind();
            Sampler2.SetActive(0);
            FilterPassShader.Uniform( FilterPassShader.GetUniformLocation("u_Texture"), 0);
            FilterPassShader.Uniform( FilterPassShader.GetUniformLocation("u_Direction"), vec2(0.0, 0.0) );
            FilterPassShader.Uniform( FilterPassShader.GetUniformLocation("u_TexelSize"), 1.0f/vec2( WINDOW_WIDTH, WINDOW_HEIGHT ));

            PlaneVAO.Draw(Primitave::TRIANGLES, 6 );
            //FilterRTT.Unbind();


            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }

        // Clear the VAO
        // Since we had flagged the array buffers for deletion ,they will now be
        // cleared as well since they are no longer bound to any VAOs
        //VAO.Release();

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

    auto gMainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

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
