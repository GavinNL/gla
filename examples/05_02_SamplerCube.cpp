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

#include <glm/gtc/noise.hpp> // for glm::perlin

#include "glad.h"
#include <gla/gla.h>
#include <GLFW/glfw3.h>

#include <gla/utils/glfw_window.h>
//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Sampler Cube"
//=================================================================================


//using namespace gla;
using namespace gla;



int main(int argc, char **argv)
{

    GLFW_Window mWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    { // create a scope around the main GL calls so that glfwTerminate is not called before
      // the gla objects are automatically destroyed.

        //================================================================
        // 1. Create the vertices of the triangle using our vertex structure
        //    The vertex strucutre contains positions and UV of each
        //    vertex in the triangle.
        //
        //    We are not going to use an index array this time.
        //
        //    We are also going to render a quad by rendering the
        //    4 vertices as a Triangle Fan
        //================================================================
        // Also create an index buffer.
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };

        std::vector< MyVertex > VertexBuffer;

        // create the vertices in counter clockwise order
        VertexBuffer.push_back( { vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f )} ); // bottom left
        VertexBuffer.push_back( { vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f )} ); // bottom right
        VertexBuffer.push_back( { vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f )} ); // top right
        VertexBuffer.push_back( { vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f )} ); // top left

        // Load the buffer into the GPU
        ArrayBuffer buff( VertexBuffer );

        VertexArray VAO;
        VAO.Attach<glm::vec3, glm::vec2>( buff );

        buff.Release();  // we can release this, because it is attached to the VAO, it will not be
                         // destroyed on the GPU until VAO is released.


        //================================================================
        // 2. Load some images and force them to use 3 colour components
        //================================================================
        Image Tex1("./resources/textures/rocks.jpg"    , 3 );
        Image Tex2("./resources/textures/rocks1024.jpg", 3 );
        Image Tex3("./resources/textures/marble.jpg"   , 3 );

        //================================================================
        // 3. Create the Texture Array
        //================================================================
        //SamplerCube Cube;
        Sampler Cube;

        // Create the GPUTexture array with 3 layers that hold 256x256 images with 3 components each, and 2 mipmaps.
        Cube.CreateTextureCubeMap( uvec2(256,256), Sampler::RGB8);

        // Resize the textures so they match the TextureArray. This will throw an exception if
        // the sizes do not match.
        Tex1.resize( uvec2(256,256) );
        Tex2.resize( uvec2(256,256) );
        Tex3.resize( uvec2(256,256) );


        // Copy the textures into the appropriate layer. This will throw an exception if
        // the number of colour channels in the TextureArray does not match what's on the Texture
        //   Note: If the Image does not match the Sampler2D dimensions, it will be rescaled
        //         to the appropriate dimensions and channels. If that is not desired
        //          use TexArray2D.SetLayer(Tex1, 0, uvec2(0,0);
        Cube[0] << Tex1; // positive x
        Cube[1] << Tex2; // negative x
        Cube[2] << Tex3; // positive y

        Tex1.g = Tex1.r;
        Tex1.b = Tex1.r;
        Tex2.g = Tex2.r;
        Tex2.b = Tex2.r;
        Tex3.g = Tex3.r;
        Tex3.b = Tex3.r;


        Cube[3] << Tex1; // negative y
        Cube[4] << Tex2; // positive z
        Cube[5] << Tex3; // negative z

        //===============================================================


        // We dont need the cpu texture anymore, so we can clear it.
        Tex1.clear();
        Tex2.clear();
        Tex3.clear();

        //================================================================
        // 4. Load the Texture Array shader
        //================================================================
        ShaderProgram TextureArrayShader = ShaderProgram::Load( "./resources/shaders/TextureCube.s" );



        // Get the IDs of the uniform variables inside the shader.
        auto uTextureArrayID = TextureArrayShader.GetUniformLocation("uTextureArray");

        //==========================================================


        // timer object
        Timer_T<float> Timer;

        while ( mWindow )
        {
            TextureArrayShader.Bind();

            // Set the TextureArray as the current texture 0;
            Cube.SetActive(0);


            // Send the Uniforms to the shader.
            TextureArrayShader.Uniform( uTextureArrayID, 0);      //which texture unit our Sampler2D array is bound to


            VAO.Draw(Primitave::TRIANGLE_FAN, 4);



            mWindow.Poll();
            mWindow.SwapBuffers();
        }
    }


    return 0;
}

