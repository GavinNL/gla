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

#include<gla/utils/cameracontrol.h>

#include <gla/utils/glfw_events.h>
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

    gla::CameraControl<GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_MOUSE_BUTTON_RIGHT> Camera_Controller;


   glfw_events Events(gMainWindow);

   Events.onKey         =  [&Camera_Controller] (int key, int action) { Camera_Controller.InsertKey(key, action); };
   Events.onMouseMove   =  [&Camera_Controller] (double x, double y) {  Camera_Controller.InsertMouse(x, y);      };
   Events.onMouseButton =  [&Camera_Controller] (int button, int action) {  Camera_Controller.InsertButton(button,action);      };

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
        //auto BoxVertices   = createSphere();  // returns a vector of vertices. It does not use an index buffer
        auto PlaneVertices = createPlane(50,50);

        auto m_BoxMesh    = m_MeshBuffer.Insert( BoxVertices.vertices, BoxVertices.indices);

//        for(auto & n : BoxVertices.vertices)
//            n.n = -n.n;

#define USEPLANE
#ifndef USEPLANE
        auto m_OutBoxMesh = m_MeshBuffer.Insert( BoxVertices.vertices, BoxVertices.indices);
#else
        auto m_OutBoxMesh = m_MeshBuffer.Insert( PlaneVertices.vertices, PlaneVertices.indices);
#endif


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
        ShaderProgram mCubeShadowMapShader        = ShaderProgram::Load( "./resources/shaders/CubeShadowMap.s" );
        ShaderProgram mShadowMapShader_ShadowPass = ShaderProgram::Load( "./resources/shaders/CubeShadowMapPass.s" );


        ShaderProgram GBufferSPass_Shader = ShaderProgram::Load( "./resources/shaders/GBuffer_SPass.s" );


        //==========================================================


        //==========================================================
        // Create the Shadow map
        //==========================================================

        Sampler ShadowTexture;
        ShadowTexture.CreateTextureCubeMap( uvec2(1024), Sampler::DEPTH_COMPONENT32 );
        ShadowTexture.ClampToEdgeX().ClampToEdgeY().ClampToEdgeZ();

        RenderToTexture mShadowMap;

        mShadowMap[RenderToTexture::DEPTH] << ShadowTexture;



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
        RTT[RenderToTexture::COLOR2] << Sampler::RGBATexture( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}    );
        RTT[RenderToTexture::DEPTH ] << Sampler::DepthTexture16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT} );

#endif
        //==========================================================================



        //================================================================
        // 5. Create a transform structure used to transform the
        //    position/orientation of the 3d object
        //================================================================
        Transform m_Box1T;
        Transform m_Box2T;
        Transform m_PlaneT;

        Camera m_Camera;
        m_Camera.Perspective( glm::radians(45.0f),(float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 1.0f, 100.0f );

        Camera_Controller.attachCamera( m_Camera );
        //m_BoxT.SetPosition(   { 0 ,     1 , -0  } );

#ifndef USEPLANE
        m_PlaneT.SetScale( vec3(30) );
#endif

        //================================================================
        // 6. Create a camera Object to help position the camera
        //================================================================
//        Camera C;
//        C.SetPosition( {1.0, 3.0, 25.0f});
//        C.Perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 1.0f, 100.0f);



        Timer_T<float> Timer;
        Timer_T<float> Timer2;

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            auto t = Timer.getElapsedTime()*0.25;

            float dt = Timer2.getElapsedTime();
            Timer2.reset();

            Camera_Controller.Calculate(dt);

            m_Box1T.SetEuler( { t, t * 0.4, -0.0 } );

#ifndef USEPLANE
            m_PlaneT.SetPosition( { 0 ,  14 ,  0 } );
#else
            m_PlaneT.SetPosition( { 0 ,  -1 ,  0 } );
#endif
            m_Box1T.SetPosition(    vec3(  cos(t), 0, sin(t) )*5.0f );
            m_Box2T.SetPosition(    vec3(  0*cos(t), 100, -0*sin(t) )*5.0f );



            vec3  m_LightPos = vec3(0,3,0);

            float m_FarPlane = 50.0f;

            //auto CameraPosition   = vec3(-4,7,-7);
            //auto CameraViewMatrix = glm::lookAt( CameraPosition , glm::vec3( 0.0, 0.0, 0.0), glm::vec3(0.0,  1.0, 0.0));
            //auto CameraProjMatrix = glm::perspective( glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 1.0f, m_FarPlane );

            auto CameraPosition   = m_Camera.GetPosition();//vec3(-4,7,-7);
            auto CameraViewMatrix = m_Camera.GetMatrix();//glm::lookAt( CameraPosition , glm::vec3( 0.0, 0.0, 0.0), glm::vec3(0.0,  1.0, 0.0));
            auto CameraProjMatrix = m_Camera.GetProjectionMatrix();//glm::perspective( glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 1.0f, m_FarPlane );

            glm::mat4 shadowProj  = glm::perspective( glm::radians(90.0f), 1.0f, 0.1f, m_FarPlane );

            std::vector<glm::mat4> m_ShadowMatrices =
            {
                shadowProj*glm::lookAt( m_LightPos, m_LightPos + glm::vec3( 1.0, 0.0, 0.0),  glm::vec3(0.0, -1.0, 0.0)) ,
                shadowProj*glm::lookAt( m_LightPos, m_LightPos + glm::vec3(-1.0, 0.0, 0.0),  glm::vec3(0.0, -1.0, 0.0)) ,
                shadowProj*glm::lookAt( m_LightPos, m_LightPos + glm::vec3( 0.0, 1.0, 0.0),  glm::vec3(0.0,  0.0, 1.0)) ,
                shadowProj*glm::lookAt( m_LightPos, m_LightPos + glm::vec3( 0.0,-1.0, 0.0),  glm::vec3(0.0,  0.0,-1.0)) ,
                shadowProj*glm::lookAt( m_LightPos, m_LightPos + glm::vec3( 0.0, 0.0, 1.0),  glm::vec3(0.0, -1.0, 0.0)) ,
                shadowProj*glm::lookAt( m_LightPos, m_LightPos + glm::vec3( 0.0, 0.0,-1.0),  glm::vec3(0.0, -1.0, 0.0))
            };

            if(1)
            {
                mShadowMap.Bind(); // Bind the RenderToTexture object (basically a framebuffer object)
                glViewport(0, 0, 1024, 1024);
                glEnable(GL_DEPTH_TEST);
                glClear( GL_DEPTH_BUFFER_BIT );
                // Shadow Pass
                //glEnable(GL_CULL_FACE);
                //glCullFace(GL_FRONT);

                mCubeShadowMapShader.Bind();



                mCubeShadowMapShader.Uniform("u_ShadowMatrices[0]", m_ShadowMatrices[0] );
                mCubeShadowMapShader.Uniform("u_ShadowMatrices[1]", m_ShadowMatrices[1] );
                mCubeShadowMapShader.Uniform("u_ShadowMatrices[2]", m_ShadowMatrices[2] );
                mCubeShadowMapShader.Uniform("u_ShadowMatrices[3]", m_ShadowMatrices[3] );
                mCubeShadowMapShader.Uniform("u_ShadowMatrices[4]", m_ShadowMatrices[4] );
                mCubeShadowMapShader.Uniform("u_ShadowMatrices[5]", m_ShadowMatrices[5] );
                mCubeShadowMapShader.Uniform("u_LightPos"     ,  m_LightPos);
                mCubeShadowMapShader.Uniform("u_FarPlane"     ,  m_FarPlane);



                mCubeShadowMapShader.Uniform("u_ModelMatrix", m_PlaneT.GetMatrix() );
                    m_OutBoxMesh.Draw();
                mCubeShadowMapShader.Uniform("u_ModelMatrix", m_Box1T.GetMatrix() );
                    m_BoxMesh.Draw();
                mCubeShadowMapShader.Uniform("u_ModelMatrix", m_Box2T.GetMatrix() );
                    m_BoxMesh.Draw();


                mShadowMap.Unbind();

           // } else {

                RTT.Bind();
                glEnable(GL_DEPTH_TEST);
                glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glCullFace(GL_BACK);
                // Use the GBufferShader
                mShadowMapShader_ShadowPass.Bind();

                // Attach the Sampler to Texture Unit 0.


                 // Tell the shader that we are using Texture Unit 0 for the sampler


                Samp1.SetActive(0);

                ShadowTexture.SetActive(1);


                mShadowMapShader_ShadowPass.Uniform( "u_FarPlane"  ,  m_FarPlane);
                mShadowMapShader_ShadowPass.Uniform( "u_ViewMatrix",  CameraViewMatrix );
                mShadowMapShader_ShadowPass.Uniform( "u_ProjMatrix",  CameraProjMatrix );
                mShadowMapShader_ShadowPass.Uniform( "u_Diffuse"   ,  0 );
                mShadowMapShader_ShadowPass.Uniform( "u_ShadowMap" ,  1 );
                mShadowMapShader_ShadowPass.Uniform( "u_LightPos"  ,  m_LightPos );
                mShadowMapShader_ShadowPass.Uniform( "u_ViewPos"   ,  CameraPosition );


                mShadowMapShader_ShadowPass.Uniform( "u_ModelMatrix", m_PlaneT.GetMatrix() );
                m_OutBoxMesh.Draw();

                mShadowMapShader_ShadowPass.Uniform( "u_ModelMatrix", m_Box1T.GetMatrix() );
                m_BoxMesh.Draw();
                mShadowMapShader_ShadowPass.Uniform( "u_ModelMatrix", m_Box2T.GetMatrix() );
                m_BoxMesh.Draw();

                //VAO.Draw(Primitave::TRIANGLES, Vertices.indices.size() );

                // Unbind the RenderToTexture so we now render to the actual screen
                RTT.Unbind();

                RTT.Texture(RenderToTexture::COLOR0).SetActive(0);//Positions.SetActive(0);
                RTT.Texture(RenderToTexture::COLOR1).SetActive(1);//Normals.SetActive(1);
                RTT.Texture(RenderToTexture::COLOR2).SetActive(2);//Colours.SetActive(2);
                RTT.Texture(RenderToTexture::DEPTH ).SetActive(3);//Depth.SetActive(3);

                mShadowMap.Texture(RenderToTexture::DEPTH).SetActive(4);//Depth.SetActive(3);
            }
            //================================================================
            // 8. Perform the pixel pass
            //================================================================
            //glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            GBufferSPass_Shader.Bind();

            // Access the individual textures by referring to them by
            // their attachment. It will throw an exception if
            // that attachment does not exist.

            GBufferSPass_Shader.Uniform( "gPosition"  , 0 );
            GBufferSPass_Shader.Uniform( "gNormal"    , 1 );
            GBufferSPass_Shader.Uniform( "gAlbedoSpec", 2 );
            GBufferSPass_Shader.Uniform( "gDepth"     , 3 );

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
