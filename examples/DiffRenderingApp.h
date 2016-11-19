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

#ifndef GLA_DIFFERED_RENDERING_APP
#define GLA_DIFFERED_RENDERING_APP

#include <stdio.h>

#define GLA_VERBOSE 1
#define GLA_INFO 1
#define GLA_TIMER 1

#include "glad.h"
#include <iostream>
#include <cstdlib>
#include <gla/gla.h>
#include <gla/eng/atlas.h>
#include <gla/eng/meshbuffer.h>

#include <GLFW/glfw3.h> // GLFW helper library

#include <gla/utils/app.h>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_TITLE  "Framebuffers and Differed Rendering"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

using namespace gla;


class DiffRenderingApp : public gla::utils::GLFW_App
{
    public:

    DiffRenderingApp(int w, int h, const char * title) : GLFW_App(w,h,title)
    {
        LoadShaders();
        SetupFrameBuffer();
    }

    virtual void onRender(double dt){}

    void onFrame(double dt)
    {
        m_FBO.Bind(); // bind the FBO so we render to the FBO textures


        this->onRender(dt);

        m_FBO.UnBind();
        DoPixelPass();
    }


    void DoPixelPass()
    {
        //================================================================
        // 8. Perform the pixel pass
        //================================================================
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Shader_PixelPass.Bind();

        m_fb_Positions.SetActive(0);
        m_fb_Normals.SetActive(1);
        m_fb_Colours.SetActive(2);
        m_fb_Depth.SetActive(3);


        m_Shader_PixelPass.Uniform( m_Shader_PixelPass.GetUniformLocation("gPosition")  , 0 );
        m_Shader_PixelPass.Uniform( m_Shader_PixelPass.GetUniformLocation("gNormal")    , 1 );
        m_Shader_PixelPass.Uniform( m_Shader_PixelPass.GetUniformLocation("gAlbedoSpec"), 2 );
        m_Shader_PixelPass.Uniform( m_Shader_PixelPass.GetUniformLocation("gDepth")     , 3 );

        m_Plane.Draw<true>(Primitave::TRIANGLE_FAN,4);
    }

    void LoadShaders()
    {
        START_TIMER()
        //m_Shader_Gbuffer   = ShaderProgram::Load( "./resources/shaders/GBuffer_Instance.s" );
        m_Shader_PixelPass = ShaderProgram::Load( "./resources/shaders/GBuffer_SPass.s" );
        END_TIMER("Loading Shaders")
    }

   void SetupFrameBuffer()
    {
        START_TIMER()
        //================================================================
        // 4. Create the Frame Buffer object
        //    we will use a frame buffer to store the following per-pixel values
        //         position
        //         normals
        //         colours
        //         depth
        //================================================================
        m_FBO.Generate();
        m_FBO.Bind();

        // Use framebuffer helper functions to create textures to be used for holding paricular types of data
        m_fb_Positions = Sampler::Vec3Texture16f( glm::uvec2 { Width(), Height() } );
        m_fb_Normals   = Sampler::Vec3Texture16f( glm::uvec2 { Width(), Height() } );
        m_fb_Colours   = Sampler::RGBATexture(    glm::uvec2 { Width(), Height() }    );
        m_fb_Depth     = Sampler::DepthTexture16f( glm::uvec2{ Width(), Height() } );

        //FBO.Bind();
        m_FBO.Attach(m_fb_Positions, FrameBuffer::COLOR0);
        m_FBO.Attach(m_fb_Normals,   FrameBuffer::COLOR1);
        m_FBO.Attach(m_fb_Colours,   FrameBuffer::COLOR2);
        m_FBO.Attach(m_fb_Depth  ,   FrameBuffer::DEPTH);
        m_FBO.Use( {FrameBuffer::COLOR0 , FrameBuffer::COLOR1, FrameBuffer::COLOR2} );

        // Check if the frame buffer is complete
        if( m_FBO.Check() != FrameBuffer::COMPLETE )
            GLA_LOGD << "ERROR!!!" << std::endl;

        m_FBO.UnBind();
        //=========================================================================
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };
        std::vector< MyVertex > VertexBuffer;

        VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,0.0) }); // 3
        VertexBuffer.push_back( { glm::vec3(-1.0, -1.0, 0.0) , glm::vec2(0.0,1.0) }); // 0
        VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,1.0) }); // 1
        VertexBuffer.push_back( { glm::vec3( 1.0,  1.0, 0.0) , glm::vec2(1.0,0.0) }); // 2

        std::vector<unsigned int> IndexBuffer;

        ArrayBuffer ab(VertexBuffer);
        m_Plane = VertexArray::MakeVAO<vec3,vec2>(  ab );

        END_TIMER("Frame Buffer Setup")
    }


    FrameBuffer m_FBO;
    Sampler m_fb_Positions;
    Sampler m_fb_Normals  ;
    Sampler m_fb_Colours  ;
    Sampler m_fb_Depth    ;

    gla::VertexArray  m_Plane; // must be drawn as a triangle fan

    ShaderProgram m_Shader_PixelPass;
};

//=============================================================================

#endif
