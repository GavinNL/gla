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


#include <gla/exper/buffers.h>
#include <gla/exper/camera.h>
#include <gla/exper/timer.h>
#include <gla/exper/framebuffer.h>
#include <gla/exper/image.h>
#include <gla/exper/sampler2d.h>
#include <gla/exper/sampler2darray.h>
#include <gla/exper/geometry.h>
#include <gla/exper/shader.h>

//#include <gla/geometry/geometry.h>
//#include <gla/global.h>
//#include <gla/camera.h>
//#include <gla/texture.h>
//#include <gla/timer.h>
//#include <gla/shader.h>
//#include <gla/gputexturearray.h>
//#include <gla/vertexarrayobject.h>
//#include <gla/framebufferobject.h>
//#include <gla/solids.h>
//#include <gla/uniformbuffer.h>


//namespace glan = gla::experimental;



#endif // GLA_H
