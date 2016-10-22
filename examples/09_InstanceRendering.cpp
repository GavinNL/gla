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

#define GLA_INFO 1
#define GLA_TIMER 1

#include "glad.h"
#include <iostream>
#include <cstdlib>
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

        START_TIMER()
            Mesh BoxVertices     = createBox( );
            Mesh ConeVertices    = createCone(1 , 0.2, 10);
            Mesh SphereVertices  = createSphere(0.5);

            m_Meshs.emplace_back( m_MeshBuffer.Insert( BoxVertices.vertices    , BoxVertices.indices) );
            m_Meshs.emplace_back( m_MeshBuffer.Insert( SphereVertices.vertices , SphereVertices.indices) );
            m_Meshs.emplace_back( m_MeshBuffer.Insert( SphereVertices.vertices , SphereVertices.indices) );

            for(auto & M : m_Meshs)
            {

                GLA_LOGI <<  "count: "           << std::setw(6) << M.count
                         <<  " |  base index: "  << std::setw(6) << M.base_index_location
                         <<  " |  base_vertex: " << std::setw(6) << M.base_vertex
                         <<  " |  vao: " << M.vao                       << std::endl;
            }
        END_TIMER("Loading Meshs")


        m_Camera.SetPosition( {0.0, -2.0, 10.0f});
        m_Camera.Perspective(45.0f, (float)Width()/(float)Height(), 0.1f);

        m_Transforms.resize(10);

        m_TransformsMat4.resize(m_Transforms.size());

        for(int i=0;i<m_Transforms.size();i++)
        {
            float R = 4;
            float t = (float(i) / float(m_Transforms.size()) )* 2*3.141592653589;
            m_Transforms[i].SetPosition( { R*cos(t),1, R*sin(t) });
        }

        // maximum 10 transforms, (see the shader)
        assert( m_Transforms.size() <= 10 );

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

        // Tell the shader that we are using Texture Unit 0 for the sampler
        m_Shader_Gbuffer.Uniform( m_Shader_Gbuffer.GetUniformLocation("uSampler"), 0 );
        m_Shader_Gbuffer.Uniform( m_Shader_Gbuffer.GetUniformLocation("uCamera"),  m_Camera.GetProjectionMatrix() * m_Camera.Camera::GetMatrix() );


        // loop through all the tranforms and get a model matrix
        // The model matrix will be stored in a contigious array so we can send
        // all at the same time
        for(int i=0;i<m_Transforms.size();i++)
        {

            float t = Timer.getElapsedTime();
            m_Transforms[i].SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * i, -0.0 } );
            m_Transforms[i].SetScale( {0.3,0.3,0.3});
            m_Transforms[i].SetPosition( { i*cos(t* 1 / (1+i) ), 0, i*sin(t * 1/(i+1)) } );

            m_TransformsMat4[i] = m_Transforms[i].GetMatrix();
        }

        static long long counter=0;
        counter++;
        DO_EVERY( 2.0 )
                GLA_LOGI << "Avg FPS: " << ( counter/ 2)<<  std::endl;
                counter=0;
        END_EVERY

        // Send all transformation matrice at once
        m_Shader_Gbuffer.Uniform( m_Shader_Gbuffer.GetUniformLocation("uTransform[0]"),  m_TransformsMat4[0],  m_TransformsMat4.size() );

        // Draw the mesh N times.

        m_Meshs[0].DrawInstanced<true>(gla::Primitave::TRIANGLES, m_TransformsMat4.size());  // bind the first one


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
        m_Shader_Gbuffer   = ShaderProgram::Load( "./resources/shaders/GBuffer_Instance.s" );
        m_Shader_PixelPass = ShaderProgram::Load( "./resources/shaders/GBuffer_SPass.s" );
        END_TIMER("Loading Shaders")
    }

    void LoadTextures()
    {
        START_TIMER()
        Image Tex1("./resources/textures/rocks.jpg",  3 );

        m_Sampler.emplace_back( Sampler2D(Tex1) );
        END_TIMER("Loading Textures")
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

        END_TIMER("Frame Buffer Setup")
    }

    gla::MeshBuffer<unsigned int, vec3,vec2,vec3> m_MeshBuffer;

    FrameBuffer m_FBO;
    Sampler2D m_fb_Positions;
    Sampler2D m_fb_Normals  ;
    Sampler2D m_fb_Colours  ;
    Sampler2D m_fb_Depth    ;

    std::vector<Sampler2D>        m_Sampler;
    std::vector<gla::Mesh_T>      m_Meshs;
    std::vector<gla::Transform>   m_Transforms;
    std::vector<glm::mat4>        m_TransformsMat4;

    gla::Camera                   m_Camera;
    gla::VertexArray              m_Plane; // must be drawn as a triangle fan

    ShaderProgram m_Shader_Gbuffer;
    ShaderProgram m_Shader_PixelPass;


};

int main()
{
    Instance_Rendering_App A(1024,768, "Instance Rendering");

    START_TIMER()
    A.run();
    END_TIMER("Total Run Time")
    return 0;
}
//=============================================================================
