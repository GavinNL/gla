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

#define GLA_LOG_ALL


#include "glad.h"
#include <gla/gla.h>
#include <GLFW/glfw3.h>

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================


//using namespace gla;
using namespace gla;



int main(int argc, char **argv)
{

    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();


    { // adding an extra scope here because we want all gla objects automatically destroyed when they go out of scope
      // calling glfwTerminate before destroying the openGL objects will cause a segfault, so putting
      // this scope here will make the gla objects destroy themselves before terminate gets called.

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);




        //================================================================
        // 1. Create the vertices of the triangle using our vertex structure
        //    The vertex strucutre contains positions and colours of each
        //    vertex in the triangle.
        //================================================================

        // Populate a standard vector of the position and colours of each vertex
        // We will use two separate buffers this time.
        std::vector< glm::vec3 > Position;
        std::vector< glm::vec4 > Colour;


        Position.push_back( glm::vec3(-1.0f, -1.0f, 0.f) );
        Position.push_back( glm::vec3( 1.0f ,-1.0f, 0.f) );
        Position.push_back( glm::vec3( 0.0f , 1.0f, 0.f) );

        Colour.push_back( glm::vec4(1.f, 0.f, 0.f, 1.0f)  );
        Colour.push_back( glm::vec4(0.f, 1.f, 0.f, 1.0f)  );
        Colour.push_back( glm::vec4(0.f, 0.f, 1.f, 1.0f)  );


        ArrayBuffer PosBuffer(Position);
        ArrayBuffer ColBuffer(Colour);

        //================================================================
        // 2. Load the Shader program from files. We will use two shaders
        //    a vertex and a fragment shader.
        //================================================================
        ShaderProgram TriangleShader;
        TriangleShader.AttachShaders( VertexShader  ("./resources/shaders/HelloTriangle.v"),
                                      FragmentShader("./resources/shaders/HelloTriangle.f") );


        PosBuffer.EnableAttributes<vec3, mat4, vec2>();

        while (!glfwWindowShouldClose(gMainWindow) )
        {

            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();


            // Tell OpenGL that each vertex
            // consists of one vec3 and one vec4 that are both un-normalized.
            // This function will automatically bind the array buffer and set the
            // attributes.
            PosBuffer.EnableAttributes<vec3>(0);  // The first attribute will be a vec3
            ColBuffer.EnableAttributes<vec4>(1);  // second attribute will be vec4

            // If you want to normalize the data, for example for surface normals, you can use
            // Which says that attribute 1 (ie: the vec4) should be noramlized data
            //== another_buffer_reference.EnableAttributes<vec3, vec4>( NormalizeFlags::_1 ); ==

            // Now draw the triangle.
            // we are drawing Triangles, starting at Vertex Index 0
            // and we are drawing 3 vertices (because 3 vertices make a triangle)
            PosBuffer.Draw<false>(Primitave::TRIANGLES, 3, 0);


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

    auto gMainWindow = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

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

