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
        // 1. Create a MeshBuffer object which holds information about
        //    a particualr type of mesh.
        //    The mesh buffer is essentially a single VertexBuffer,
        //    a single IndexBuffer wraped up inot a VertexArray Object
        //    You can store multiple different drawable meshes in it
        //    as long they all have the same vertex attributes.
        //
        //    Using this reduces the number of times you need to bind
        //    different buffers and reduces the CPU->GPU calls
        //
        //================================================================
        // Create an indexed mesh buffer which will be used to hold vertices with 3 attributes:
        //   vec3 - position,   vec2 - UV coords,   vec3 - normals
        //
        // and use an unsigned int as the index type
        gla::MeshBuffer< unsigned int, vec3, vec2, vec3> MB;


        MB.ReserveIndices(10000); // Allocate enough memory on the GPU to hold 10,000 vertices
        MB.ReserveVertices(1000); // Allocate enough memory on the GPu to hold 1000 indices

        //====================== Create the geometry for the box ==============================
        // These all exist on the CPU, not on the GPU
        Mesh CylVertices    = createCylinder(0.2f, 10);
        Mesh SphereVertices = createSphere(0.5);
        //=====================================================================================


        // Add each of the meshs to the buffer
        // Returns a Mesh_T type which can be used to draw
        //
        gla::Mesh_T SphereMesh = MB.Insert( SphereVertices.vertices , SphereVertices.indices);
        gla::Mesh_T CylMesh    = MB.Insert( CylVertices.vertices    , CylVertices.indices);

        {
            gla::Mesh_T CylMesh2    = MB.Insert( CylVertices.vertices    , CylVertices.indices);
            // CylMesh2 should free itself after this!
        }



        //================================================================
        // The rest of all the initialization is the same as the
        // FrameBuffer example. Procede to the render loop to see
        // what's different.
        //================================================================

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
        // 4. Create the Frame Buffer object
        //    we will use a frame buffer to store the following per-pixel values
        //         position
        //         normals
        //         colours
        //         depth
        //================================================================
        FrameBuffer FBO;
        FBO.Generate();
        FBO.Bind();

        // Use framebuffer helper functions to create textures to be used for holding paricular types of data
        Sampler2D Positions = FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        Sampler2D Normals   = FrameBuffer::CreateBufferTexture_Vec3_16f( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        Sampler2D Colours   = FrameBuffer::CreateBufferTexture_RGBA(     glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );
        Sampler2D Depth     = FrameBuffer::CreateBufferTexture_Depth16F( glm::uvec2{WINDOW_WIDTH, WINDOW_HEIGHT}  );

        //FBO.Bind();
        FBO.Attach(Positions, FrameBuffer::COLOR0);
        FBO.Attach(Normals,   FrameBuffer::COLOR1);
        FBO.Attach(Colours,   FrameBuffer::COLOR2);
        FBO.Attach(Depth  ,   FrameBuffer::DEPTH);

        FBO.Use( {FrameBuffer::COLOR0 , FrameBuffer::COLOR1, FrameBuffer::COLOR2} );

        // Check if the frame buffer is complete
        if( FBO.Check() != FrameBuffer::COMPLETE )
            std::cout << "ERROR!!!" << std::endl;
        //==========================================================================



        //================================================================
        // 5. Create a transform structure used to transform the
        //    position/orientation of the 3d object
        //================================================================
        Transform T1;
        Transform T2;

        T1.SetPosition( { 1,0,-3});
        T2.SetPosition( {-1,0,-3});

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
            FBO.Bind(); // bind the FBO so we render to the FBO textures

            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use the GBufferShader
            GBufferShader.Bind();

            // Attach the Sampler to Texture Unit 0.
            Samp1.SetActive(0);

            T1.SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * 0.4, -0.0 } );
            T2.SetEuler( { Timer.getElapsedTime(), Timer.getElapsedTime() * 0.4, -0.0 } );

            // Tell the shader that we are using Texture Unit 0 for the sampler
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uSampler"), 0 );
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uCamera"),  C.GetProjectionMatrix() );

            //================================================================
            // 7.1. Draw the two meshs
            //
            //  Each of these meshs belong to the same VertexArrayObject.
            //  So you only need to bind the first one you draw, by setting
            //  the template paramter to true. Setting it to false will
            //  not bind the VAO.
            //================================================================
            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uTransform"),  T1.GetMatrix() );
            SphereMesh.Draw<true>();  // bind the first one

            GBufferShader.Uniform( GBufferShader.GetUniformLocation("uTransform"),  T2.GetMatrix() );
            CylMesh.Draw<false>();    // no need to bind again.

            // Unbind the FBO so we now render to the actual screen
            FBO.UnBind();

            //================================================================
            // 8. Perform the pixel pass
            //================================================================
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            GBufferSPass_Shader.Bind();

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
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gAlbedoSpec"), i );
            GBufferSPass_Shader.Uniform( GBufferSPass_Shader.GetUniformLocation("gDepth")     , 3 );

            PlaneVAO.Draw(Primitave::TRIANGLES, 6 );

            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }

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
