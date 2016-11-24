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
#include <glm/gtc/noise.hpp>

#include <GLFW/glfw3.h> // GLFW helper library

#include <locale>

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "Uniform Buffers"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

using namespace gla;

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    { // create a scope around the main GL calls so that glfwTerminate is not called before
        // the gla objects are automatically destroyed.

        //================================================================
        // 1. Create the vertices of the triangle using our vertex structure
        //    The vertex strucutre contains positions and UV of each
        //    vertex in the triangle.
        //================================================================
        struct MyVertex
        {
            vec3 p;
            vec2 uv;
        };

        std::vector< MyVertex > VertexBuffer;

        VertexBuffer.push_back( { vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f )} );
        VertexBuffer.push_back( { vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f )} );
        VertexBuffer.push_back( { vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f )} );
        VertexBuffer.push_back( { vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f )} );

        // Load the buffer into the GPU
        ArrayBuffer buff( VertexBuffer );


        VertexArray VAO = VertexArray::MakeVAO<vec3,vec2>( buff );

        // Load some textures. And force using 3 components (r,g,b)
        Image Tex1("./resources/textures/rocks.jpg",  3 );
        Image Tex2("./resources/textures/rocks1024.jpg", 3 );
        Image Tex3(256,256, 3);


        buff.Release();


        //=========================================================================================
        //2. Create the Texture array
        //=========================================================================================
        Sampler TexArray2D;

        // Create the GPUTexture array with 3 layers that hold 256x256 images with 3 components each, and 2 mipmaps.
        TexArray2D.CreateTexture2DArray( uvec2(256,256), 3, Sampler::RGB8);

        // Set the red channel using a lambda function
        Tex3.r =  IMAGE_EXPRESSION( perlin( vec2(x,y) * 4.0f, vec2(4.0,4.0) )*0.5 + 0.5f )

                // Resize the textures so they match the TextureArray. This will throw an exception if
                // the sizes do not match.
                Tex1.resize( uvec2(256,256) );
        Tex2.resize( uvec2(256,256) );


        // Copy the textures into the appropriate layer. This will throw an exception if
        // the number of colour channels in the TextureArray does not match what's on the Texture
        //   Note: If the Image does not match the Sampler2D dimensions, it will be rescaled
        //         to the appropriate dimensions and channels. If that is not desired
        //          use TexArray2D.SetLayer(Tex1, 0, uvec2(0,0);
        TexArray2D[0] << Tex1;
        TexArray2D[1] << Tex2;
        TexArray2D[2] << Tex3;

        // Alternatively, one can use the following
        //   TexArray2D.SetLayer( Tex1, 0, uvec2(0 ,0) );
        //   TexArray2D.SetLayer( Tex2, 0, uvec2(0 ,0) );
        //   TexArray2D.SetLayer( Tex3, 0, uvec2(0 ,0) );

        //===============================================================


        // we dont need the cpu texture anymore, so we can clear it.
        Tex1.clear();
        Tex2.clear();
        Tex3.clear();



        //================================================================
        // 3. Load the shader
        //================================================================
        ShaderProgram UniformBufferShader = ShaderProgram::Load( "./resources/shaders/UniformBuffer.s"  );


        //================================================================
        // 4. Create the Uniform Buffer
        //================================================================
        // This is the structure that we will create a buffer for
        // Note: the alignment of structs and the alignment of uniform blocks in the shader
        // are NOT the same. vec3 are are aligned to vec4, so you will need to pad
        // the data with an extra by te
        struct UniformBufferStruct140 {
            vec3          Dir_Speed; // x,y = direction, z = speed
            float         padding1;  // we need this padding because vec3 in the shader is aligned to vec4, that is the amount of space
            vec4          colour;
        };
        UniformBufferStruct140 UniformData; // Create an instance of the struct in memory.


        // Now create uniform buffer.
        UniformBuffer uniform_buffer;

        // Reserve just enough space to hold the structure
        uniform_buffer.Reserve( sizeof(UniformBufferStruct140) );

        // We first need to get the block index of the "Uniform140" block in the shader.
        auto BlockIndex = UniformBufferShader.GetUniformBlockIndex("Uniform140");

        // Get the block size;
        auto BlockSize  = UniformBufferShader.GetUniformBlockSize("Uniform140");

        // If these two are not the same, then it means some of the
        // member variables in UniformBufferStruct140 are miss aligned
        assert( BlockSize == sizeof(UniformBufferStruct140) );


        std::cout << "Max Buffer Bind Points          : " << UniformBuffer::Get_MaxUniformBufferBindings() << std::endl;
        std::cout << "Max Buffer block size           : " << UniformBuffer::Get_MaxUniformBlockSize() << std::endl;
        std::cout << "Max Combined Uniform block size : " << UniformBuffer::Get_MaxCombinedUniformBlocks() << std::endl;


        // Bind the UniformBuffer to the UniformBlock in the shader.
        // Both must be bound to the same location.
        uniform_buffer.SetBindPoint(1);
        UniformBufferShader.BindUniformBlock(BlockIndex, 1);
        //--------------------------------------------------------------------

        Timer_T<float> Timer;
        while (!glfwWindowShouldClose(gMainWindow) )
        {

            // Set the GPU as the current texture 0;
            int TextureIndex = 0;
            TexArray2D.SetActive(TextureIndex);

            auto t = Timer.getElapsedTime();

            UniformData.Dir_Speed = vec3( cos(t), sin(t), cos(t)*sin(t) );
            UniformData.colour    = vec4( cos(t)*cos(t), 0,0,1 );

            // Copy the data over by using the () operator
            // The UniformBufferShader will now contain the new values of UniformData
            uniform_buffer( UniformData );

            UniformBufferShader.Uniform( UniformBufferShader.GetUniformLocation("uTextureArray"), TextureIndex);

            VAO.Draw(Primitave::TRIANGLE_FAN, 4);

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
