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

#define GLA_LOGGING 1

#include "glad.h"

#include <gla/gla.h>
#include <gla/eng/atlas.h>
#include <gla/eng/mesh_buffer.h>

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


class Instance_Rendering_App : public gla::utils::GLFW_App
{
    public:

    Instance_Rendering_App(int w, int h, const char * title) : GLFW_App(w,h,title)
    {
        m_MeshBuffer.ReserveIndices(1000000);
        m_MeshBuffer.ReserveVertices(1000000);

        //Mesh BoxVertices    = createBox( );
        Mesh BoxVertices    = createCone(1 , 0.2, 10);
        Mesh SphereVertices = createSphere(0.5);

        m_Meshs.emplace_back( m_MeshBuffer.Insert( SphereVertices.vertices , SphereVertices.indices) );
        m_Meshs.emplace_back( m_MeshBuffer.Insert( BoxVertices.vertices    , BoxVertices.indices) );


        for(auto & M : m_Meshs)
        {
            //GLA_LOG <<  "first: " << M.first               << std::endl;
            GLA_LOG <<  "count: " << M.count               << std::endl;
            GLA_LOG <<  "base index: " << M.base_index_location << std::endl;
            GLA_LOG <<  "base_vertex: " << M.base_vertex         << std::endl;
            GLA_LOG <<  "vao: " << M.vao                 << std::endl;
        }

        m_Camera.SetPosition( {0.0,0.0,0.0f});
        m_Camera.Perspective(45.0f, (float)Width()/(float)Height(), 0.1f);

        m_Transforms.resize(2);

        m_Transforms[0].SetPosition( { 1,0,-3});
        m_Transforms[1].SetPosition( {-1,0,-3});

        LoadShaders();
        SetupFrameBuffer();
        LoadTextures();
    }

    virtual void OnKey(int k, int scancode, int action ,int mods)
    {

    }

    void onFrame(double dt)
    {
        static gla::Timer_T<float> Timer;

        m_FBO.Bind(); // bind the FBO so we render to the FBO textures

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, Width(), Height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the GBufferShader
        m_Shader_Gbuffer.Bind();

        // Attach the Sampler to Texture Unit 0.
        m_Sampler[0].SetActive(0);

        m_Transforms[0].SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * 0.4, -0.0 } );
        m_Transforms[1].SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * 0.4, -0.0 } );

        // Tell the shader that we are using Texture Unit 0 for the sampler
        m_Shader_Gbuffer.Uniform( m_Shader_Gbuffer.GetUniformLocation("uSampler"), 0 );
        m_Shader_Gbuffer.Uniform( m_Shader_Gbuffer.GetUniformLocation("uCamera"),  m_Camera.GetProjectionMatrix() );

        //================================================================
        // 7.1. Draw the two meshs
        //
        //  Each of these meshs belong to the same VertexArrayObject.
        //  So you only need to bind the first one you draw, by setting
        //  the template paramter to true. Setting it to false will
        //  not bind the VAO.
        //================================================================
        m_Shader_Gbuffer.Uniform( m_Shader_Gbuffer.GetUniformLocation("uTransform"),  m_Transforms[0].GetMatrix() );
        m_Meshs[0].Draw<true>();  // bind the first one

        m_Shader_Gbuffer.Uniform( m_Shader_Gbuffer.GetUniformLocation("uTransform"),  m_Transforms[1].GetMatrix() );
        m_Meshs[1].Draw<false>();    // no need to bind again.

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
        m_Shader_Gbuffer   = ShaderProgram::Load( "./resources/shaders/GBuffer.s" );
        m_Shader_PixelPass = ShaderProgram::Load( "./resources/shaders/GBuffer_SPass.s" );
    }

    void LoadTextures()
    {
        Image Tex1("./resources/textures/rocks.jpg",  3 );

        m_Sampler.emplace_back( Sampler2D(Tex1) );
    }

    void SetupFrameBuffer()
    {
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
        m_fb_Positions = FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{Width(), Height() }  );
        m_fb_Normals   = FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{Width(), Height() }  );
        m_fb_Colours   = FrameBuffer::CreateBufferTexture_RGBA(     glm::uvec2{Width(), Height() }  );
        m_fb_Depth     = FrameBuffer::CreateBufferTexture_Depth16F( glm::uvec2{Width(), Height() }  );

        //FBO.Bind();
        m_FBO.Attach(m_fb_Positions, FrameBuffer::COLOR0);
        m_FBO.Attach(m_fb_Normals,   FrameBuffer::COLOR1);
        m_FBO.Attach(m_fb_Colours,   FrameBuffer::COLOR2);
        m_FBO.Attach(m_fb_Depth  ,   FrameBuffer::DEPTH);
        m_FBO.Use( {FrameBuffer::COLOR0 , FrameBuffer::COLOR1, FrameBuffer::COLOR2} );

        // Check if the frame buffer is complete
        if( m_FBO.Check() != FrameBuffer::COMPLETE )
            std::cout << "ERROR!!!" << std::endl;

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


    }

    gla::MeshBuffer<unsigned int, vec3,vec2,vec3> m_MeshBuffer;

    FrameBuffer m_FBO;
    Sampler2D m_fb_Positions;
    Sampler2D m_fb_Normals  ;
    Sampler2D m_fb_Colours  ;
    Sampler2D m_fb_Depth    ;

    std::vector<Sampler2D>        m_Sampler;
    std::vector<gla::Mesh_T> m_Meshs;
    std::vector<gla::Transform>   m_Transforms;

    gla::Camera                   m_Camera;
    gla::VertexArray              m_Plane; // must be drawn as a triangle fan

    ShaderProgram m_Shader_Gbuffer;
    ShaderProgram m_Shader_PixelPass;


};

int main()
{
    Instance_Rendering_App A(1024,768, "Instance Rendering");

    A.run();
    return 0;
}
//=============================================================================
