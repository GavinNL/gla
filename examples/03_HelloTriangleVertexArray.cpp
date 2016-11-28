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

#include "glad.h"
#include <gla/gla.h>
#include <GLFW/glfw3.h>

#include <gla/utils/glfw_window.h>
//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Hello Triangle - Vertex Array Object"
//=================================================================================


//using namespace gla;
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
        //    The vertex strucutre contains positions and colours of each
        //    vertex in the triangle.
        //================================================================
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec4 c;
        };

        std::vector< MyVertex > VertexBuffer;
        VertexBuffer.push_back( { glm::vec3(-1.0f, -1.0f, 0.f), glm::vec4(1.f, 0.f, 0.f, 1.0f)  }  );
        VertexBuffer.push_back( { glm::vec3( 1.0f ,-1.0f, 0.f), glm::vec4(0.f, 1.f, 0.f, 1.0f)  }  );
        VertexBuffer.push_back( { glm::vec3( 0.0f , 1.0f, 0.f), glm::vec4(0.f, 0.f, 1.f, 1.0f)  }  );

        // Send to GPU
        ArrayBuffer         G( VertexBuffer );

        //================================================================
        // 2. Create an index buffer that will indicate which vertices
        //    in the Array_Buffer will make up the triangles
        //================================================================
        std::vector< glm::uvec3 > IndexBuffer;
        IndexBuffer.push_back( glm::uvec3( 0 ,1, 2) );

        // Send the index buffer to the GPU
        ElementArrayBuffer  E( IndexBuffer );

        //================================================================
        // 3. Create the VertexArrayObject.
        //================================================================
        VertexArray VAO;
        VAO.Attach<glm::vec3, glm::vec4>( G, E );

        // We can no use the VAO objec to draw the triangle



        //================================================================
        // 4. Load the triangle shader
        //================================================================
        ShaderProgram TriangleShader;
        TriangleShader.AttachShaders(  VertexShader("./resources/shaders/HelloTriangle.v"),
                                       FragmentShader("./resources/shaders/HelloTriangle.f")  );

        //====

        while ( mWindow )
        {

            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            // Bind the VertexBuffer and tell openGL to draw 3 indices (3 indices make 1 triangle)
            VAO.Draw( Primitave::TRIANGLES, 3 );


            mWindow.Poll();
            mWindow.SwapBuffers();
        }
    }


    return 0;
}


