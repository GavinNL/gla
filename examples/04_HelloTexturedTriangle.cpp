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

#include <glm/gtc/noise.hpp>

#include "glad.h"
#include <gla/gla.h>
#include <GLFW/glfw3.h>

#include <gla/utils/glfw_window.h>
//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Hello Textured Triangle"
//=================================================================================



using namespace gla;


int main(int argc, char **argv)
{

    GLFW_Window mWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

      { // adding an extra scope here because we want all gla objects automatically destroyed when they go out of scope
      // calling glfwTerminate before destroying the openGL objects will cause a segfault, so putting
      // this scope here will make the gla objects destroy themselves before terminate gets called.

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


        //================================================================
        // 1. Create the vertices of the triangle using our vertex structure
        //    The vertex strucutre contains positions and UV coords of each
        //    vertex in the triangle.
        //    We will use a vertex array object
        //================================================================
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };

        std::vector< MyVertex >     VertexData;
        std::vector< unsigned int > IndexData;
        VertexData.push_back( { glm::vec3(-1.0f, -1.0f, 0.f), glm::vec2(0.f, 1.f)  }  );
        VertexData.push_back( { glm::vec3( 1.0f ,-1.0f, 0.f), glm::vec2(1.f, 1.f)  }  );
        VertexData.push_back( { glm::vec3( 0.0f , 1.0f, 0.f), glm::vec2(.5f, .0f)  }  );

        IndexData.push_back(0);
        IndexData.push_back(1);
        IndexData.push_back(2);
        // Send the vertex data to the GPU.
        ArrayBuffer        G( VertexData );
        ElementArrayBuffer E( IndexData );

        // Create a VertexArray from the data
        VertexArray VAO = VertexArray::MakeVAO<glm::vec3, glm::vec2>( G, E);

        //================================================================


        //================================================================
        // 2. Load an image we want to use as the texture
        //================================================================

        Image Img;

        Img.loadFromPath("./resources/textures/rocks1024.jpg" );


        // A texture in GLSL is called a Sampler2D, we send the data to the GPU
        // by creating a Sampler2D object and initializing it with the Image object
        Sampler MyTexture(Img);


        // We can modify the red channel using a lambda function
        Img.r = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*8.0f ) + 0.5);  };

        // Or similarly using the macro which essentially does the same thing as the above
        // but reduces the amount you need to write
        Img.b = IMAGE_EXPRESSION( glm::clamp(2*x + y,0.0f,1.0f) );

        // We can update the sampler with the new data
        MyTexture.Copy( Img, 0, uvec2(0,0) );


        //================================================================
        // 3. Load the shader we want to use when drawing the triangle
        //         Can use one or the other.
        //  The .s shader file has each component of the shader
        //  separated by xml tags:  <vertex>, <fragment> <geometry> <tessellation_control>
        //  and <tessellation_evaluation>
        //
        //  You can also provide a string map to automatically replace
        //  text in the shader to something else
        //   map<std::string, std::string> Replace;
        //  Replace["
        //================================================================
        auto TriangleShader = ShaderProgram::Load("./resources/shaders/Textures.s");

        Sampler Test;
        Test.CreateTexture2D( Img.size(), Sampler::RGB8 );

        Test << Img; // copy the Image into the Sampler
        //================================================================

        while ( mWindow )
        {
            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            // Attach the Sampler to Texture Unit 0.
            Test.SetActive(0);

            // Tell the shader that we are using Texture Unit 0 for the sampler
            TriangleShader.Uniform( TriangleShader.GetUniformLocation("uSampler")  , 0           );
            TriangleShader.Uniform( TriangleShader.GetUniformLocation("uTransform"), glm::mat4() );

            // Draw the triangle.
            VAO.Draw(Primitave::TRIANGLES, 3);
            mWindow.Poll();
            mWindow.SwapBuffers();
        }
    }


    return 0;
}

