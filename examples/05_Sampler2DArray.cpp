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

#include <glm/gtc/noise.hpp> // for glm::perlin

#include <GLFW/glfw3.h>



using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Sampler 2D Array"
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
        Image Tex1("./resources/textures/rocks.jpg"    ,  3 );
        Image Tex2("./resources/textures/rocks1024.jpg", 3 );
        Image Tex3(256,256, 3); // empty texture

        // Set the red channel using a lambda function
        Tex3.r =  IMAGE_EXPRESSION( glm::perlin( vec2(x,y) * 4.0f, vec2(4.0,4.0) )*0.5 + 0.5f )

        //================================================================
        // 3. Create the Texture Array
        //================================================================
        Sampler2DArray TexArray2D;

        // Create the GPUTexture array with 3 layers that hold 256x256 images with 3 components each, and 2 mipmaps.
        TexArray2D.Create( uvec2(256,256), 3, 3 , 2);


        // Resize the textures so they match the TextureArray. This will throw an exception if
        // the sizes do not match.
        Tex1.resize( uvec2(256,256) );
        Tex2.resize( uvec2(256,256) );


        // Copy the textures into the appropriate layer. This will throw an exception if
        // the number of colour channels in the TextureArray does not match what's on the Texture
        //   Note: If the Image does not match the Sampler2D dimensions, it will be rescaled
        //         to the appropriate dimensions and channels. If that is not desired
        //          use TexArray2D.SetLayer(Tex1, 0, uvec2(0,0);
        TexArray2D[0] = Tex1;
        TexArray2D[1] = Tex2;
        TexArray2D[2] = Tex3;

        // Alternatively, one can use the following
        //   TexArray2D.SetLayer( Tex1, 0, uvec2(0 ,0) );
        //   TexArray2D.SetLayer( Tex2, 1, uvec2(0 ,0) );
        //   TexArray2D.SetLayer( Tex3, 2, uvec2(0 ,0) );

        //===============================================================


        // we dont need the cpu texture anymore, so we can clear it.
        Tex1.clear();
        Tex2.clear();
        Tex3.clear();



        //================================================================
        // 4. Load the Texture Array shader
        //================================================================
        ShaderProgram TextureArrayShader = ShaderProgram::Load(  "./resources/shaders/TextureArray.s" );

        // Get the IDs of the uniform variables inside the shader.
        auto uTextureArrayID = TextureArrayShader.GetUniformLocation("uTextureArray");
        auto uSpeedID        = TextureArrayShader.GetUniformLocation("uSpeed");
        //==========================================================


        // timer object
        Timer_T<float> Timer;

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            TextureArrayShader.Bind();

            // Set the TextureArray as the current texture 0;
            TexArray2D.SetActive(0);


            vec2 Speed = Timer.getElapsedTime() * vec2(0.15,0.4) * 0.2f;

            // Send the Uniforms to the shader.
            TextureArrayShader.Uniform( uTextureArrayID, 0);      //which texture unit our Sampler2D array is bound to
            TextureArrayShader.Uniform( uSpeedID ,        Speed); //a constant variable that we are using in the shader


            VAO.Draw(Primitave::TRIANGLE_FAN, 4);


            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    //VAO.clear();


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
