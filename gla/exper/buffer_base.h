#ifndef EXP_BUFFER_BASE
#define EXP_BUFFER_BASE


#include "handle.h"
#include "types.h"

#include <typeinfo>
#include <type_traits>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <tuple>
#include <array>
#include <type_traits>





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
    using BaseType    = typename NextElement::value_type;

    int num_components = sizeof( NextElement ) / sizeof( typename NextElement::value_type);

    const int SizeOfBaseType = sizeof(BaseType);

    GLenum GlBaseType;

    if( std::is_floating_point<BaseType>::value )
    {

        if( SizeOfBaseType == sizeof(std::int32_t) )
        {
            GlBaseType = GL_FLOAT;
        }
        else if( SizeOfBaseType == sizeof(std::int64_t) )
        {
            GlBaseType = GL_DOUBLE;
        } else {
            throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
        }
    }
    else
    {

        if( std::is_signed<BaseType>::value )
        {
            if( SizeOfBaseType == sizeof(std::int8_t) )  GlBaseType = GL_BYTE;
            else if( SizeOfBaseType == sizeof(std::int16_t) ) GlBaseType = GL_SHORT;
            else if( SizeOfBaseType == sizeof(std::int32_t) ) GlBaseType = GL_INT;
            else {
                throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
            }
        } else {
            if( SizeOfBaseType == sizeof(std::int8_t) )  GlBaseType = GL_UNSIGNED_BYTE;
            if( SizeOfBaseType == sizeof(std::int16_t) ) GlBaseType = GL_UNSIGNED_SHORT;
            if( SizeOfBaseType == sizeof(std::int32_t) ) GlBaseType = GL_UNSIGNED_INT;
            else {
                throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
            }
        }

    }
    //std::cout << "Current Offset      : " << (long long)offset << std::endl;
    //std::cout << "Size of next element: " << sizeof(NextElement) << std::endl;
    //std::cout << "Size of int : " << sizeof(int) << std::endl;

    #define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))


     glVertexAttribPointer( GLuint(I),
                            num_components ,
                            GlBaseType,
                            normalized[I] ,
                            sizeof(TupleType),
                            BUFFER_OFFSET(offset));
    glEnableVertexAttribArray(I);

 //   std::cout << "glVertexAttribPointer(" << I << ", ";
 //   std::cout << num_components << ", ";
 //   std::cout << "GL_FLOAT" << ", ";
 //   std::cout << (normalized[I] ? "GL_TRUE" : "GL_FALSE") << ", ";
 //   std::cout << sizeof(TupleType) << ", ";
 //   std::cout << (long long)offset << ");" << std::endl;

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
            m_Size = data.size()*sizeof(VertexData);
        }

        void Bind( ) const
        {
            glBindBuffer( static_cast<GLuint>(target) , Get() );
        }

        void Unbind( ) const
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
        void EnableAttributes(  const std::array<bool, std::tuple_size< std::tuple< GLM_Vec_Types...> >::value > & normalized ) const
        //static void EnableAttributes(  const bool & normalized)
        {
            Bind();
            gla::experimental::EnableVertexAttribArrayFromTuple<0, std::tuple<GLM_Vec_Types...> > (0, normalized);
        }

        template <typename... GLM_Vec_Types>
        /**
         * @brief EnableAttributes
         * Enables attributes  assuming all attributes are unnormalized
         */
        void EnableAttributes() const
        {
            Bind();
            std::array<bool, std::tuple_size< std::tuple< GLM_Vec_Types...> >::value > FalseArray;
            FalseArray.fill(false);
            gla::experimental::EnableVertexAttribArrayFromTuple<0, std::tuple<GLM_Vec_Types...> > (0, FalseArray);
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
            m_Size = size_in_bytes;
        }

        std::size_t m_Size;

};

/*
class Array_Buffer : public Buffer<BufferBindTarget::ARRAY_BUFFER>
{
    public:
        template<typename VertexData>
        Array_Buffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(data, usage)
        {
        }

        template<bool BindFirst=true>
        void Draw( Primitave p, std::size_t first, std::size_t NumberOfPrimitaves)
        {
            if(BindFirst) Bind();

            glDrawArrays( static_cast<GLenum>(p),  static_cast<GLint>(first),  static_cast<GLsizei>(NumberOfPrimitaves) );
        }
};


class Element_Array_Buffer : public Buffer<BufferBindTarget::ELEMENT_ARRAY_BUFFER>
{
    public:
        template<typename VertexData>
        Element_Array_Buffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(data, usage)
        {

                 if(   std::is_same<VertexData,glm::i32vec4>::value
                    || std::is_same<VertexData,glm::i32vec3>::value
                    || std::is_same<VertexData,glm::i32vec2>::value
                    || std::is_same<VertexData,std::int32_t>::value
                    || std::is_same<VertexData,glm::i32vec1>::value) { m_Data = DataType::INT; return;}

            else if(   std::is_same<VertexData,glm::u32vec4>::value
                    || std::is_same<VertexData,glm::u32vec3>::value
                    || std::is_same<VertexData,glm::u32vec2>::value
                    || std::is_same<VertexData,std::uint32_t>::value
                    || std::is_same<VertexData,glm::u32vec1>::value) { m_Data = DataType::UNSIGNED_INT; return;}

            else if(   std::is_same<VertexData,glm::i8vec4>::value
                    || std::is_same<VertexData,glm::i8vec3>::value
                    || std::is_same<VertexData,glm::i8vec2>::value
                    || std::is_same<VertexData,std::int8_t>::value
                    || std::is_same<VertexData,glm::i8vec1>::value) { m_Data = DataType::BYTE; return;}

            else if(   std::is_same<VertexData,glm::u8vec4>::value
                    || std::is_same<VertexData,glm::u8vec3>::value
                    || std::is_same<VertexData,std::uint8_t>::value
                    || std::is_same<VertexData,glm::u8vec2>::value
                    || std::is_same<VertexData,glm::u8vec1>::value) { m_Data = DataType::UNSIGNED_BYTE; return;}

            else if(   std::is_same<VertexData,glm::i16vec4>::value
                    || std::is_same<VertexData,glm::i16vec3>::value
                    || std::is_same<VertexData,glm::i16vec2>::value
                    || std::is_same<VertexData,std::int16_t>::value
                    || std::is_same<VertexData,glm::i16vec1>::value) { m_Data = DataType::SHORT; return;}

            else if(   std::is_same<VertexData,glm::u16vec4>::value
                    || std::is_same<VertexData,glm::u16vec3>::value
                    || std::is_same<VertexData,glm::u16vec2>::value
                    || std::is_same<VertexData,std::uint16_t>::value
                    || std::is_same<VertexData,glm::u16vec1>::value) { m_Data = DataType::UNSIGNED_SHORT; return;}


            throw std::runtime_error("Non standard data used for element buffer.");

        }

        template<bool BindFirst=true>
        void Draw( Primitave p, std::size_t NumberOfIndices, std::size_t first=0 )
        {
            if(BindFirst) Bind();

            //#define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))
            glDrawElements( static_cast<GLenum>(p),
                            static_cast<GLsizei>(NumberOfIndices),
                            static_cast<GLenum>(m_Data),
                            static_cast<char*>(0)+first
                            );
        }

        DataType m_Data;
};
*/
}}

#endif
