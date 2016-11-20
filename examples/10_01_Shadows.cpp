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
        // 1. Use some of the helper functions to create vertices for
        //    some basic geometric objects
        //
        //================================================================
        MeshBuffer<unsigned int, vec3, vec2, vec3> m_MeshBuffer;
        m_MeshBuffer.ReserveIndices(10000);
        m_MeshBuffer.ReserveVertices(10000);

        auto BoxVertices   = createBox();  // returns a vector of vertices. It does not use an index buffer
        //auto BoxVertices   = createSphere();
        auto PlaneVertices = createPlane(50,50);

        auto m_BoxMesh   = m_MeshBuffer.Insert( BoxVertices.vertices, BoxVertices.indices);
        auto m_PlaneMesh = m_MeshBuffer.Insert( PlaneVertices.vertices, PlaneVertices.indices);


        // Load some textures. And force using 3 components (r,g,b)
        Image Tex1("./resources/textures/rocks.jpg",  3 );

        // send the image to the GPU
        Sampler Samp1(Tex1);

        //================================================================
        // 2. Create the plane to use during the second render pass
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
        // 3. Load the two shaders we are going to use
        //       - The G Buffer Shader used to draw geometry
        //       - The Shader pass
        //================================================================
        ShaderProgram mShadowMapShader            = ShaderProgram::Load( "./resources/shaders/ShadowMap.s" );
        ShaderProgram mShadowMapShader_ShadowPass = ShaderProgram::Load( "./resources/shaders/ShadowMapPass.s" );

        ShaderProgram GBufferShader       = ShaderProgram::Load( "./resources/shaders/GBuffer.s" );
        ShaderProgram GBufferSPass_Shader = ShaderProgram::Load( "./resources/shaders/GBuffer_SPass.s" );


        //==========================================================


        //==========================================================
        // Create the Shadow map
        //==========================================================
        RenderToTexture mShadowMap;
        auto mShadowMapTexture = Sampler::DepthTexture16f( glm::uvec2{1024, 1024} );
        mShadowMapTexture.ClampToEdge();
        //mShadowMap.CreateTexture( RenderToTexture::DEPTH , glm::uvec2{1024, 1024}, RenderToTexture::depth_16f);
        mShadowMap[RenderToTexture::DEPTH] << mShadowMapTexture;//mShadowMapTexture;
        mShadowMap.Bind();


        //================================================================
        // 4. We will use the RenderToTexture class to help construct
        // the framebuffers and textures needed to render geometry
        // to a texture.
        //================================================================
        RenderToTexture RTT;

#if 0
        // Create two textures that will hold some kind fo floating point value
        RTT.CreateTexture( RenderToTexture::COLOR0, glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::vec3_16f);
        RTT.CreateTexture( RenderToTexture::COLOR1, glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::vec3_16f);

        // create a texture that will hold the colour value
        RTT.CreateTexture( RenderToTexture::COLOR2, glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::rgba);


        // Create a depth texture that will render the depth information to
        RTT.CreateTexture( RenderToTexture::DEPTH , glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}, RenderToTexture::depth_16f);

#else

        // Can also attach samplers to each target manually. The RTT will store a refernce to the samplers
        // so they won't be destroyed
        RTT[RenderToTexture::COLOR0] << Sampler::Vec3Texture16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT} );
        RTT[RenderToTexture::COLOR1] << Sampler::Vec3Texture16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT} );
        RTT[RenderToTexture::COLOR2] << Sampler::RGBATexture(    glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}    );
        RTT[RenderToTexture::DEPTH ] << Sampler::DepthTexture16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT} );

#endif
        //==========================================================================



        //================================================================
        // 5. Create a transform structure used to transform the
        //    position/orientation of the 3d object
        //================================================================
        Transform m_BoxT;
        Transform m_Box2T;
        Transform m_PlaneT;


        vec3 LightPosition        = glm::vec3(-2.0f, 4.0f, -1.0f);
        GLfloat near_plane        = 1.0f, far_plane = 500.5f;

        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView       = glm::lookAt(LightPosition,
                                          glm::vec3( 0.0f, 0.0f,  0.0f),
                                          glm::vec3( 0.0f, 1.0f,  0.0f));




        m_BoxT.SetPosition(   { 0 ,     1 , -0  } );
        m_PlaneT.SetPosition( { 0 ,    -1 , -0 } );

        //================================================================
        // 6. Create a camera Object to help position the camera
        //================================================================
        Camera C;
        C.SetPosition( {1.0, 3.0, 10.0f});
        C.Perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f);


        Timer_T<float> Timer;

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            auto t = Timer.getElapsedTime();

            m_BoxT.SetEuler( { t, t * 0.4, -0.0 } );


            // Set up the light matrix.
            glm::vec3 lPos    = glm::vec3(4.0f*cos(t), 1.2 , 4.0f*sin(t));
            glm::vec3 lLookat = m_BoxT.GetPosition(); // always look at the rotating box
            glm::vec3 lUp     = glm::vec3( 0,1,0 );

            glm::mat4 LightMatrix = glm::perspective(90.0f, 1.0f, 1.0f, 500.0f) * glm::lookAt( lPos, lLookat, lUp );

            auto CameraProj = C.GetProjectionMatrix();
            auto CameraView = C.GetMatrix();

            if(1)
            {
                mShadowMap.Bind(); // Bind the RenderToTexture object (basically a framebuffer object)
                glViewport(0, 0, 1024, 1024);
                glEnable(GL_DEPTH_TEST);
                glClear( GL_DEPTH_BUFFER_BIT );
                // Shadow Pass

                glCullFace(GL_FRONT);



                mShadowMapShader.Bind();


                mShadowMapShader.Uniform( mShadowMapShader.GetUniformLocation("u_LightMatrix"), LightMatrix );

                m_Box2T.SetPosition( lPos*1.2f);


                mShadowMapShader.Uniform( mShadowMapShader.GetUniformLocation("u_ModelMatrix"), m_PlaneT.GetMatrix() );
                    m_PlaneMesh.Draw();
                mShadowMapShader.Uniform( mShadowMapShader.GetUniformLocation("u_ModelMatrix"), m_BoxT.GetMatrix() );
                    m_BoxMesh.Draw();



                mShadowMap.Unbind();


                RTT.Bind();

                glEnable(GL_DEPTH_TEST);
                glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glCullFace(GL_BACK);

                // Use the GBufferShader
                mShadowMapShader_ShadowPass.Bind();



                 // Tell the shader that we are using Texture Unit 0 for the sampler
                mShadowMapShader_ShadowPass.Uniform( "u_LightSpaceMatrix", LightMatrix );
                mShadowMapShader_ShadowPass.Uniform( "u_ViewMatrix",       CameraView );
                mShadowMapShader_ShadowPass.Uniform( "u_ProjMatrix",       CameraProj );




                mShadowMap.Texture(RenderToTexture::DEPTH).SetActive(1);
                Samp1.SetActive(0);

                mShadowMapShader_ShadowPass.Uniform( "u_Diffuse"   , 0 );
                mShadowMapShader_ShadowPass.Uniform( "u_ShadowMap" , 1 );
                mShadowMapShader_ShadowPass.Uniform( "u_LightPos",  lPos);
                mShadowMapShader_ShadowPass.Uniform( "u_ViewPos" ,  C.GetPosition() );


                mShadowMapShader_ShadowPass.Uniform( "u_ModelMatrix", m_PlaneT.GetMatrix() );
                m_PlaneMesh.Draw();

                mShadowMapShader_ShadowPass.Uniform( "u_ModelMatrix", m_BoxT.GetMatrix() );
                m_BoxMesh.Draw();

                mShadowMapShader_ShadowPass.Uniform( "u_ModelMatrix", m_Box2T.GetMatrix() );
                m_BoxMesh.Draw();

                // Unbind the RenderToTexture so we now render to the actual screen
                RTT.Unbind();

            }
            //================================================================
            // 8. Perform the pixel pass
            //================================================================

            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            GBufferSPass_Shader.Bind();

            // Access the individual textures by referring to them by
            // their attachment. It will throw an exception if
            // that attachment does not exist.
            RTT.Texture(RenderToTexture::COLOR0).SetActive(0);//Positions.SetActive(0);
            RTT.Texture(RenderToTexture::COLOR1).SetActive(1);//Normals.SetActive(1);
            RTT.Texture(RenderToTexture::COLOR2).SetActive(2);//Colours.SetActive(2);
            RTT.Texture(RenderToTexture::DEPTH ).SetActive(3);//Depth.SetActive(3);

            mShadowMap.Texture(RenderToTexture::DEPTH).SetActive(4);//Depth.SetActive(3);

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
