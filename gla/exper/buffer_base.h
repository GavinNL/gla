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
    SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
    UNIFORM_BUFFER        = GL_UNIFORM_BUFFER,
    COPY_READ_BUFFER      = GL_COPY_READ_BUFFER,
    COPY_WRITE_BUFFER     = GL_COPY_WRITE_BUFFER
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





struct GenBuff
{
    void operator()(GLuint & x)
    {
        glGenBuffers(1, &x);

        std::cout << "Buffer Generated: " << x << std::endl;
        std::cout << "Error: " << glGetError() << std::endl;
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
class Buffer : public BaseHandle<GLuint, GenBuff, DestBuff>
{
    public:

        std::size_t m_Size  = 0;

        Buffer() : BaseHandle<GLuint, GenBuff,DestBuff>()
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


        void Bind(BufferBindTarget targ = target) const
        {
            glBindBuffer( static_cast<GLenum>(targ), Get() );
        }

        void Unbind(BufferBindTarget targ = target ) const
        {
            glBindBuffer( static_cast<GLenum>(targ) , 0 );
        }

        std::size_t Size() const {
            return m_Size;
        }
        //================================================

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


        /**
         * @brief CopyData
         * @param data   - pointer to the data
         * @param size   - size of the data
         * @param offset - offset from the start of the buffer to write to
         */
        void CopyData(const GLvoid * data, std::size_t size, std::size_t offset=0)
        {
            if( !(*this) )
            {
                Allocate( size+offset );
            }

            Bind();

            if( size+offset > m_Size )
                throw std::runtime_error("Buffer is too small to hold all the data");

            glBufferSubData( static_cast<GLenum>(target), offset, size, data);
        }

        /**
         * @brief CopyData
         * @param V
         * @param offset
         *
         * Copies a vector a data into the buffer.
         */
        template<typename VertexType>
        void CopyData(const std::vector<VertexType> & V, std::size_t offset = 0)
        {
            CopyData( (const GLvoid *)&V[0], sizeof(VertexType)*V.size(), offset);
        }


};


class Buffer_T : public BaseHandle<GLuint, GenBuff, DestBuff>
{
    public:

        void Bind(BufferBindTarget target) const
        {
            glBindBuffer(static_cast<GLenum>(target) , Get() );
        }

        void Unbind( BufferBindTarget target ) const
        {
            glBindBuffer( static_cast<GLenum>(target) , 0 );
        }

        void Allocate(std::size_t size, BufferUsage usage = BufferUsage::STATIC_DRAW)
        {
            Generate(); // generates a new handle, releases the old one.

            Bind(BufferBindTarget::COPY_READ_BUFFER);

            glBufferData( static_cast<GLenum>(BufferBindTarget::COPY_READ_BUFFER) ,
                          size,
                          NULL ,
                          static_cast<GLenum>(usage) );
            m_Size = size;
            Unbind(BufferBindTarget::COPY_READ_BUFFER);
        }

        template<typename VertexType>
        void CopySubData(const std::vector<VertexType> & V, std::size_t offset = 0)
        {
            CopyData( (unsigned char*)&V[0], sizeof(VertexType)*V.size(), offset);
        }

        void CopyData(const GLvoid * data, std::size_t size, std::size_t offset=0)
        {
            if( !(*this) )
            {
                Allocate( size + offset );
            }

            Bind(BufferBindTarget::COPY_READ_BUFFER);

            if( size+offset > m_Size )
                throw std::runtime_error("Buffer is too small to hold all the data");

            glBufferSubData( static_cast<GLenum>(BufferBindTarget::COPY_READ_BUFFER), offset, size, data);

            Unbind(BufferBindTarget::COPY_READ_BUFFER);
        }


        std::size_t Size() const { return m_Size; }

    protected:
        std::size_t m_Size  = 0;
};



}}

#endif
