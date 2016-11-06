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

#define GLA_VERBOSE 1
#define GLA_INFO 1
#define GLA_TIMER 1
#define GLA_ERROR 1
#define GLA_DEBUG 1

#include "glad.h"
#include <iostream>
#include <cstdlib>
#include <gla/gla.h>
#include <gla/eng/atlas.h>
#include <gla/eng/meshbuffer.h>

#include <GLFW/glfw3.h> // GLFW helper library


#include <gla/utils/app.h>
#include "DiffRenderingApp.h"
using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_TITLE  "Framebuffers and Differed Rendering"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

using namespace gla;


class Instance_Rendering_App : public DiffRenderingApp
{
    public:

    Instance_Rendering_App(int w, int h, const char * title) : DiffRenderingApp(w,h,title)
    {
        // Load the Instance shader using the GBuffer approach of rendering
        // to textures
        m_MultiDrawShader   = ShaderProgram::Load( "./resources/shaders/GBuffer_Instance.s" );

        // Load textures
        Image Tex1("./resources/textures/rocks.jpg",  3 );
        m_Sampler.emplace_back( Sampler2D(Tex1) );

        m_MeshBuffer.ReserveIndices(1000000);
        m_MeshBuffer.ReserveVertices(1000000);

        START_TIMER()
            Mesh BoxVertices     = createBox( glm::vec3(1.45) );
            Mesh SphereVertices  = createSphere(1.0);
            Mesh ConeVertices    = createCone(1 , 0.2, 10);

            m_Meshs.emplace_back( m_MeshBuffer.Insert( SphereVertices.vertices , SphereVertices.indices) );
            m_Meshs.emplace_back( m_MeshBuffer.Insert( BoxVertices.vertices    , BoxVertices.indices) );
            m_Meshs.emplace_back( m_MeshBuffer.Insert( ConeVertices.vertices , ConeVertices.indices) );


        END_TIMER("Loading Meshs")


        m_Camera.SetPosition( {0.0, -2.0, 10.0f});
        m_Camera.Perspective(45.0f, (float)Width()/(float)Height(), 0.1f);

        m_Transforms.resize(10);

        m_TransformsMat4.resize(m_Transforms.size());


        std::vector<int> drawids;
        for(int i=0 ; i<m_Transforms.size();i++)
        {
            float R = 4;
            float t = (float(i) / float(m_Transforms.size()) )* 2*3.141592653589;
            m_Transforms[i].SetPosition( { R*cos(t),1, R*sin(t) });
            drawids.push_back(i);
        }

        // maximum 10 transforms, (see the shader)
        assert( m_Transforms.size() <= 10 );



        //=====================================================================================================
        // The Mesh Buffer uses a single arraybuffer to store
        // the positions,normals and UV coords. Each of these attributes
        // are listed in the shader under input layout 0,1 and 2.
        //
        // We are going to use a separate array buffer to sure
        // the instance values, they will be under layout attribute 3 (see the shader source)
        //
        //
        //=====================================================================================================
        m_DrawIDBuffers.CopyData( drawids );

        m_MeshBuffer.Bind(); // bind the mesh buffer (it's a VAO)
        m_DrawIDBuffers.EnableDivisor< int >(3, 1); // tell openGL that layout 3 will consist of a single int
                                                    // that will be incremented once per instance call
                                                    // because the VAO is bound, this attribute will be enabled
                                                    // for all meshes in this meshbuffer

        m_DrawIDBuffers.Unbind();                   // unbind the VAO so that we dont accidently make changes to it.


        m_MeshBuffer.Unbind();

    }


    void onRender(double dt)
    {
        static gla::Timer_T<float> Timer;

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, Width(), Height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the GBufferShader
        m_MultiDrawShader.Bind();

        // Attach the Sampler to Texture Unit 0.
        m_Sampler[0].SetActive(0);

        // Tell the shader that we are using Texture Unit 0 for the sampler
        m_MultiDrawShader.Uniform( m_MultiDrawShader.GetUniformLocation("uSampler"), 0 );
        m_MultiDrawShader.Uniform( m_MultiDrawShader.GetUniformLocation("uCamera"),  m_Camera.GetProjectionMatrix() * m_Camera.Camera::GetMatrix() );


        // loop through all the tranforms and get a model matrix
        // The model matrix will be stored in a continious array so we can send
        // all at the same time
        for(int i=0;i<m_Transforms.size();i++)
        {

            float t = Timer.getElapsedTime();
            m_Transforms[i].SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * i, -0.0 } );
            m_Transforms[i].SetScale( {0.3,0.3,0.3});
            m_Transforms[i].SetPosition( { i*cos(t* 1 / (1+i) ), 0, i*sin(t * 1/(i+1)) } );

            m_TransformsMat4[i] = m_Transforms[i].GetMatrix();
        }


        // Send all transformation matrice at once
        m_MultiDrawShader.Uniform( m_MultiDrawShader.GetUniformLocation("uTransform[0]"),  m_TransformsMat4[0],  m_TransformsMat4.size() );


        //=====================================================================================================
        // Create a command vector for the MultiDrawElementsIndirect function
        //
        // MultidrawElementsIndirect can draw multple meshes from the same buffer, we are going to draw
        // two different meshes.
        // The uniform values for each instance of the mesh is stored in an array in the shader
        // called uTransform[], to determine which index we need to use we use a vertex attribute called
        // "inDrawID" (see the shader code).  This value is updated once per instance, rather that once
        // per vertex. (Refer to the Constructor to see how this is set up)
        //=====================================================================================================
        // Draw the mesh N times.
        std::vector<gla::MultiDrawElementsIndirectCommand> cmd(2);

        cmd[0].count         = m_Meshs[0].count;                                       // number of triangles to draw
        cmd[0].baseVertex    = m_Meshs[0].base_vertex;                                 // the base vertex to start from
        cmd[0].firstIndex    = m_Meshs[0].base_index_location/sizeof(unsigned int);    // the first INDEX (not byte location)
        cmd[0].instanceCount = 4;                                                      // number of times to draw this mesh
        cmd[1].baseInstance  = 0;                                                      // which Instance value to start form

        cmd[1].count         = m_Meshs[1].count;
        cmd[1].baseVertex    = m_Meshs[1].base_vertex;
        cmd[1].firstIndex    = m_Meshs[1].base_index_location/sizeof(unsigned int);
        cmd[1].instanceCount = 3;
        cmd[1].baseInstance  = 4;

        m_MeshBuffer.MultiDraw( cmd );


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

    gla::ArrayBuffer              m_DrawIDBuffers;
    gla::Camera                   m_Camera;


    ShaderProgram m_MultiDrawShader;


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
