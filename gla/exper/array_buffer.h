#ifndef EXP_Vertex_Buffer
#define EXP_Vertex_Buffer


#include "handle.h"
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <tuple>
#include <array>
#include <type_traits>


#include <gla/glad.h>


namespace gla { namespace experimental
{

// GPU object

enum class BufferBindTarget
{
    ARRAY_BUFFER          = GL_ARRAY_BUFFER,
    ELEMENT_ARRAY_BUFFER  = GL_ELEMENT_ARRAY_BUFFER,
    TEXTURE_BUFFER        = GL_TEXTURE_BUFFER,
    SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER
};

enum class BufferUsage
{
        STREAM_DRAW    = GL_STREAM_DRAW,
        STREAM_READ    = GL_STREAM_READ,
        STREAM_COPY    = GL_STREAM_COPY,
        STATIC_DRAW    = GL_STATIC_DRAW,
        STATIC_READ    = GL_STATIC_READ,
        STATIC_COPY    = GL_STATIC_COPY,
        DYNAMIC_DRAW   = GL_DYNAMIC_DRAW,
        DYNAMIC_READ   = GL_DYNAMIC_READ,
        DYNAMIC_COPY   = GL_DYNAMIC_COPY

};


// Terminating case
template<std::size_t I = 0, typename TupleType, std::size_t tuplesize>
static inline typename std::enable_if<I == std::tuple_size<TupleType>::value, int>::type
EnableVertexAttribArrayFromTuple(std::int32_t offset, const std::array<bool, tuplesize > & normalized)
{
    return 0;
}



// A recursive template function
// That will enable the vertex attribute based on the GLM types in a
// tuple.
// Do not call this function directly.
template<std::size_t I = 0, typename TupleType, std::size_t tuplesize>
static inline typename std::enable_if< I < std::tuple_size<TupleType>::value, int>::type
EnableVertexAttribArrayFromTuple( std::int32_t offset, const std::array<bool, tuplesize > & normalized)
{
    using NextElement = typename std::tuple_element<I, TupleType>::type;

    int num_components = sizeof( NextElement ) / sizeof( typename NextElement::value_type);

    //std::cout << "Current Offset      : " << (long long)offset << std::endl;
    //std::cout << "Size of next element: " << sizeof(NextElement) << std::endl;
//    std::cout << "Size of int : " << sizeof(int) << std::endl;

    #define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))


     glVertexAttribPointer( GLuint(I),
                            num_components ,
                            GL_FLOAT,
                            normalized[I] ,
                            sizeof(TupleType),
                            BUFFER_OFFSET(offset));

    std::cout << "glVertexAttribPointer(" << I << ", ";
    std::cout << num_components << ", ";
    std::cout << "GL_FLOAT" << ", ";
    std::cout << (normalized[I] ? "GL_TRUE" : "GL_FALSE") << ", ";
    std::cout << sizeof(TupleType) << ", ";
    std::cout << (long long)offset << ");" << std::endl;

    EnableVertexAttribArrayFromTuple<I+1, TupleType>( offset + sizeof(NextElement) , normalized );
}

struct GenBuff
{
    void operator()(GLuint & x)
    {
        static int i=0;
        x = ++i;
        glGenBuffers(1, &x);
        std::cout << "Buffer Generated: " << x << std::endl;
    }
};

struct DestBuff
{
    void operator()(GLuint & x)
    {
        std::cout << "Destroying Buffer: " << x << std::endl;
        glDeleteBuffers( 1, &x );
        x = 0;
    }
};



template<BufferBindTarget target>
/**
 * @brief The Buffer class
 *
 * A buffer stored on the GPU that holds data.
 */
class Buffer : public BaseHandle<GLuint, GenBuff,DestBuff>
{
    public:

        Buffer()
        {
        }

        Buffer(std::size_t size_in_bytes)
        {
            Allocate(size_in_bytes);
        }

        template<typename VertexData>
        Buffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW)
        {
            Generate();
            Bind();
            glBufferData( static_cast<GLenum>(target) , data.size()*sizeof(VertexData), data.data() , static_cast<GLenum>(usage) );
        }

        void Bind( )
        {
            glBindBuffer( static_cast<GLuint>(target) , Get() );
        }

        void Unbind( )
        {
            glBindBuffer( static_cast<GLuint>(target) , 0 );
        }
        //================================================

       // void BindAs( BufferBindTarget tar)
       // {
       //     std::cout << "glBindBuffer( static_cast<GLuint>(tar) )" << std::endl;
       // }

        template <typename... GLM_Vec_Types>
        /**
         * @brief EnableAttributes
         * @param normalized
         *
         * Enable vertex attributes using GLM types. For example:
         *
         *
         * EnableAttributes<glm::vec3, glm::vec2, glm::vec3>( {false, false, true}  )
         *
         *
         */
        static void EnableAttributes(  const std::array<bool, std::tuple_size< std::tuple< GLM_Vec_Types...> >::value > & normalized )
        {
            gla::experimental::EnableVertexAttribArrayFromTuple<0, std::tuple<GLM_Vec_Types...> > (0, normalized);
        }


        template<typename VertexType>
        static Buffer<target> FromData( std::vector<VertexType> & V , BufferUsage usage = BufferUsage::STATIC_DRAW)
        {
            Buffer<target> B;
            B.Bind();

            glBufferData( static_cast<GLenum>(target) , V.size()*sizeof(VertexType), V.data() , static_cast<GLenum>(usage) );

            return B;
        }

        // Allocates new data and releases old data
        // if the old data is referenced by another instance, it will remain in memory
        // until all references have been released.
        void Allocate(std::size_t size_in_bytes, BufferUsage usage = BufferUsage::STATIC_DRAW)
        {
            Generate(); // generates a new handle, releases the old one.
            Bind();
            glBufferData( static_cast<GLenum>(target) , size_in_bytes, NULL , static_cast<GLenum>(usage) );
        }
};


using Array_Buffer         = Buffer<BufferBindTarget::ARRAY_BUFFER>;            // used for vertex data
using Element_Array_Buffer = Buffer<BufferBindTarget::ELEMENT_ARRAY_BUFFER>;    // used for index data

}}

#endif
