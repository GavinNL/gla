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

#include <stdio.h>

#include "glad.h"

#include <gla/gla.h>
#include <gla/eng/sampler2dfilter.h>
#include <GLFW/glfw3.h> // GLFW helper library



/**
 * This is the same as 11_01_Filters, but uses the
 * Sampler2DFilter helper class to perform the filters
 */


using namespace gla;

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
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

        // Create a 2d filter object. Only one needs to be created
        // and can be reused multiple times. Do not create more than one
        // since all shaders/geometry are stored within the object
        Sampler2DFilter s2df;
        s2df.Init();


        // Load some textures. And force using 3 components (r,g,b)
        Image Tex1("./resources/textures/rocks1024.jpg",  3 );
        // send the image to the GPU
        Sampler2D RocksTexture(Tex1);

        // Create two textures that will hold the filtered versions of RocksTexture
        auto Sampler1 = Sampler2D::RGBTexture(Tex1.size());
        auto Sampler2 = Sampler2D::RGBTexture(Tex1.size());

        // First run the gaussian filter in the y direction
        // and saving the output to Sampler1
        s2df.Gaussian( RocksTexture, Sampler1,  vec2(0.0,1.0) );

        // Now run the gaussian filter in the x direction
        // and saving it to Sampler2
        s2df.Gaussian( Sampler1, Sampler2,      vec2(1.0,0.0) );

        // do it a bunch more times so we can acutaly see the blurring
        s2df.Gaussian( Sampler2, Sampler1, vec2(0.0,1.0) );
        s2df.Gaussian( Sampler1, Sampler2, vec2(1.0,0.0) );


        s2df.Gaussian( Sampler2, Sampler1, vec2(0.0,1.0) );
        s2df.Gaussian( Sampler1, Sampler2, vec2(1.0,0.0) );


        s2df.Gaussian( Sampler2, Sampler1, vec2(0.0,1.0) );
        s2df.Gaussian( Sampler1, Sampler2, vec2(1.0,0.0) );

        s2df.Gaussian( Sampler2, Sampler1, vec2(0.0,1.0) );
        s2df.Gaussian( Sampler1, Sampler2, vec2(1.0,0.0) );

        s2df.Gaussian( Sampler2, Sampler1, vec2(0.0,1.0) );
        s2df.Gaussian( Sampler1, Sampler2, vec2(1.0,0.0) );

        s2df.Gaussian( Sampler2, Sampler1, vec2(0.0,1.0) );
        s2df.Gaussian( Sampler1, Sampler2, vec2(1.0,0.0) );

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            // The Null filter without any arguments, will simply draw the texture
            // in full screen.
            s2df.Null(Sampler2);


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

    auto gMainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

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
