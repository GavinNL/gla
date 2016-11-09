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
        // 1. Use some of the helper functions to create vertices for
        //    some basic geometric objects
        //
        //================================================================
#define BOX
#ifdef BOX

        auto Vertices = createBox();  // returns a vector of vertices. It does not use an index buffer

#else

        auto Vertices = createCylinder(0.2f, 5); // returns a mesh structure, which contains vertices and index info

#endif
        ArrayBuffer        Buff( Vertices.vertices );
        ElementArrayBuffer Ind(  Vertices.indices );

        VertexArray VAO = VertexArray::MakeVAO<vec3,vec2,vec3>( Buff, Ind );


        // Load some textures. And force using 3 components (r,g,b)
        Image Tex1("./resources/textures/rocks.jpg",  3 );

        // send the image to the GPU
        Sampler2D Samp1(Tex1);

        //================================================================
        // 2. Create the plane to use during the second render pass
        //================================================================
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


        //================================================================
        // 3. Load the two shaders we are going to use
        //       - The G Buffer Shader used to draw geometry
        //       - The Shader pass
        //================================================================
        ShaderProgram GBufferShader = ShaderProgram::Load( "./resources/shaders/GBuffer.s" );


        ShaderProgram GBufferSPass_Shader = ShaderProgram::Load( "./resources/shaders/GBuffer_SPass.s" );

         //==========================================================


        //================================================================
        // 4. We will use the RenderToTexture class to help construct
        // the framebuffers and textures needed to render geometry
        // to a texture.
        //================================================================
        RenderToTexture RTT;

        // Create two textures that will hold some kind fo floating point value
        RTT.CreateTexture( RenderToTexture::COLOR0, glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::vec3_16f);
        RTT.CreateTexture( RenderToTexture::COLOR1, glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::vec3_16f);

        // create a texture that will hold the colour value
        RTT.CreateTexture( RenderToTexture::COLOR2, glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::rgba);

        // Create a depth texture that will render the depth information to
        RTT.CreateTexture( RenderToTexture::DEPTH , glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::depth_16f);


        //==========================================================================



        //================================================================
        // 5. Create a transform structure used to transform the
        //    position/orientation of the 3d object
        //================================================================
        Transform T;
        T.SetPosition( { 0 , 0 , -3 } );

        //================================================================
        // 6. Create a camera Object to help position the camera
        //================================================================
        Camera C;
        C.SetPosition( {0.0,0.0,0.0f});
        C.Perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f);



        Timer_T<float> Timer;
        Timer_T<float> Timer2;

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            //================================================================
            // 7. Perform the GBuffer pass
            //================================================================
            RTT.Bind(); // bind the FBO so we render to the FBO textures

            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use the GBufferShader
            GBufferShader.Bind();

            // Attach the Sampler to Texture Unit 0.
            Samp1.SetActive(0);

            T.SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * 0.4, -0.0 } );

            // Tell the shader that we are using Texture Unit 0 for the sampler
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uSampler"), 0 );
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uTransform"),  T.GetMatrix() );
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uCamera"),  C.GetProjectionMatrix() );

            // Draw the 3d Object

            VAO.Draw(Primitave::TRIANGLES, Vertices.indices.size() );

            // Unbind the FBO so we now render to the actual screen
            RTT.Unbind();

            //================================================================
            // 8. Perform the pixel pass
            //================================================================
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            GBufferSPass_Shader.Bind();

            RTT[RenderToTexture::COLOR0].SetActive(0);//Positions.SetActive(0);
            RTT[RenderToTexture::COLOR1].SetActive(1);//Normals.SetActive(1);
            RTT[RenderToTexture::COLOR2].SetActive(2);//Colours.SetActive(2);
            RTT[RenderToTexture::DEPTH] .SetActive(3);//Depth.SetActive(3);

            static int i=0;
            if( Timer2.getElapsedTime() > 2.0 )
            {
                i = (i+1)%4;
                Timer2.reset();
            }

            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gPosition")  , 0 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gNormal")    , 1 );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gAlbedoSpec"), i );
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
