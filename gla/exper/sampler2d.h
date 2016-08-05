#ifndef EXP_Sample2d
#define EXP_Sample2d


#include "handle.h"
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <tuple>
#include <array>
#include <type_traits>

#include <GL/glew.h>
#include <GL/gl.h>

namespace gla { namespace experimental
{

struct GenSampler2D
{
    void operator()(GLuint & x)
    {
        static int i=0;
        x = ++i;
        //std::cout << "glGenBuffers(1, &m_ID)" << std::endl;
        std::cout << "Sampler2D Generated: " << x << std::endl;
    }
};

struct DestSampler2D
{
    void operator()(GLuint & x)
    {
        std::cout << "Sampler2D destroyed: " << x << std::endl;
        x = 0;
    }
};



template<BufferBindTarget target>
/**
 * @brief The Buffer class
 *
 * A buffer stored on the GPU that holds data.
 */
class Sampler2D : public BaseHandle<GLuint, GenSampler2D, DestSampler2D>
{
    public:

        Sampler2D()
        {
        }

        Sampler2D(std::size_t size_in_bytes)
        {
            Allocate(size_in_bytes);
        }

        void Bind( )
        {
            //glBindBuffer( static_cast<GLuint>(target) , Get() );
        }

        void Unbind( )
        {
        //    glBindBuffer( static_cast<GLuint>(target) , 0 );
        }

        void SetActive(std::size_t location)
        {

        }

        //================================================


        void Allocate(  )
        {
          //  Generate(); // generates a new handle, releases the old one.
          //  Bind();
          //  glBufferData( static_cast<GLenum>(target) , size_in_bytes, NULL , static_cast<GLenum>(usage) );
        }
};



}}

#endif
