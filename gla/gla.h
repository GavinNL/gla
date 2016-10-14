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


/*! \mainpage GLA: openGL Assistant
 *
 * \section intro_sec Introduction
 *
 * GLA is a library to help work with OpenGL core objects. I built this library while learning modern OpenGL 3+ and mostly
 * use it for my own experimentations, but I am releasing it to the public in case anyone else might find it useful. GLA
 * allows easy creation and manipulation of OpenGL core objects such as.
 *
 * GLA is broken up into two libraries, the Core library and the Engine library. The Core library is purely a library for
 * working with OpenGL objects for example:
 *
 * \li \ref TextureExample_sec "Textures"
 * \li \ref TextureArraysExample_sec "Texture Arrays"
 * \li \ref ArrayBuffersExample_sec "Array Buffers"
 * \li \ref VertexArrayObjectsExample_sec "Vertex Array Objects"
 * \li \ref TransformationsExample_sec "Transformations"
 * \li \ref ShadersExample_sec "Shaders"
 * \li \ref PrimitaveExample_sec "Primitave geometry (spheres, planes, cones, etc)"
 * \li \ref FrameBufferObjectExample_sec "FrameBuffer Objects"
 *
 * The Engine library is optional and has other methods/class that can help in designing a game or any other graphical application.
 * It is not required. It provides the following:
 *
 * \li TransformationSequences
 * \li Model Loading
 * \li Bone Hierarchy
 * \li Scene Management
 * \li Materials
 *
 * GLA is NOT a full-fledge rendering engine like Ogre3d or Irrlicht, it does NOT provide any of the following, although
 * you can use GLA to create these things yourself. Some of the items in the following list may apper as a module.
 *
 * \li Scene management
 * \li Automatic lighting/shadows/etc
 * \li Game specific objects. Eg: terrains, skyboxes, etc
 * \li Audio
 * \li Physics
 * \li User Interfaces
 * \li User Input
 *
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * \section example_sec Examples
 *
 * \subsection Example1 Example 1: Hello Triangle
 *
 * \subsection Example2 Example 2: Textures and Texture Manipulation

 *
 * \section license_sec License
 *
 * \page page_examples Examples
 *
 * \section HelloTriangle_sec Hello Triangle
 *  Hello this is an examples page
 *
 * \section TextureExample_sec Textures
 *   \code{.cpp}
 *      Texture T(256,256);          // Create a blank texture 256x256
 *
 *      // set the red channel of the texture using a lambda function
 *      // The lambda function needs to return a float ranging between 0 and 1 and input a const vec2&
 *      T.r = [] (const vec2 & r) { return( cos(glm::length( r-vec2(0.5f))*3.14159); };
 *
 *
 *      // Set teh pixel values of the texture using a lambda function
 *      // The lambda function needs to return a vec4 with values ranging between 0 and 1
 *      T   = [] (const vec2 & r)
 *          {
 *                 float red   =  cos(glm::length( r-vec2(0.5f))*3.14159 );
 *                 float green =  sin(glm::length( r-vec2(0.5f))*3.14159 );
 *                 return( vec4(red,green,1.0,1.0)) ;
 *          };
 *
 *      GPUTexture TexGpu = T.toGPU();    // send the texture to the GPU so that it can be used in openGL calls.
 *    \endcode
 *
 *
 *
 * \section TextureArrayExample_sec Texture Arrays
 *   \code{.cpp}
 *      Texture T(256,256);          // Create a blank texture 256x256
 *
 *      // set the red channel of the texture using a lambda function
 *      // The lambda function needs to return a float ranging between 0 and 1 and input a const vec2&
 *      T.r = [] (const vec2 & r) { return( cos(glm::length( r-vec2(0.5f))*3.14159); };
 *
 *
 *      // Set teh pixel values of the texture using a lambda function
 *      // The lambda function needs to return a vec4 with values ranging between 0 and 1
 *      T   = [] (const vec2 & r)
 *          {
 *                 float red   =  cos(glm::length( r-vec2(0.5f))*3.14159 );
 *                 float green =  sin(glm::length( r-vec2(0.5f))*3.14159 );
 *                 return( vec4(red,green,1.0,1.0)) ;
 *          };
 *
 *      GPUTexture TexGpu = T.toGPU();    // send the texture to the GPU so that it can be used in openGL calls.
 *    \endcode
 */

#ifndef GLA_H
#define GLA_H

#include <gla/core/timer.h>
#include <gla/core/buffers.h>
#include <gla/core/camera.h>

#include <gla/core/image.h>
#include <gla/core/sampler2d.h>
#include <gla/core/sampler2darray.h>

#include <gla/core/framebuffer.h>
#include <gla/core/geometry.h>
#include <gla/core/shader.h>


namespace gla
{
    namespace core
    {
        using namespace experimental;
    }
}




#endif // GLA_H
