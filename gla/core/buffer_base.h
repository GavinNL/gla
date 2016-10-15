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


struct BufferInfo
{
    std::size_t m_Offset  = 0;
    std::size_t m_Reserve = 0;
};

//template<BufferBindTarget target>
/**
 * @brief The Buffer class
 *
 * A buffer stored on the GPU that holds data.
 */
class Buffer : public BaseHandle<GLuint, GenBuff, DestBuff, BufferInfo>
{
    public:

        Buffer() : BaseHandle<GLuint, GenBuff,DestBuff,BufferInfo>()
        {
        }

        Buffer(std::size_t size_in_bytes) : BaseHandle<GLuint, GenBuff,DestBuff, BufferInfo>()
        {
            Reserve(size_in_bytes);
        }


        template<typename VertexData>
        Buffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : BaseHandle<GLuint, GenBuff,DestBuff,BufferInfo>()
        {
            Generate();
            Bind(  *this , BufferBindTarget::COPY_WRITE_BUFFER);
            glBufferData( static_cast<GLenum>(BufferBindTarget::COPY_WRITE_BUFFER) , data.size()*sizeof(VertexData), data.data() , static_cast<GLenum>(usage) );
            SharedData().m_Reserve   = data.size()*sizeof(VertexData);
            SharedData().m_Offset    = data.size()*sizeof(VertexData);
            Unbind(BufferBindTarget::COPY_WRITE_BUFFER);
        }

        static void Bind(const Buffer & B, BufferBindTarget targ)
        {
            glBindBuffer( static_cast<GLenum>(targ), B.Get() );
        }

        static void Unbind(BufferBindTarget targ)
        {
            glBindBuffer( static_cast<GLenum>(targ), 0);
        }


        void CopyBufferData( const Buffer & Other, std::size_t read_offset, std::size_t write_offset, std::size_t size)
        {
            if( !Other )
            {
                throw std::runtime_error("Other buffer does not exist, there is no data to copy over.");
            }
            if( !(*this) )
            {
                Reserve( size );
            }

            Bind( *this, BufferBindTarget::COPY_WRITE_BUFFER );
            Bind( Other, BufferBindTarget::COPY_READ_BUFFER );

            if( read_offset+size >= Size() )
            {
                throw std::runtime_error("Attempting to copy data into buffer of smaller size");
            }

            glCopyBufferSubData(static_cast<GLenum    >( BufferBindTarget::COPY_READ_BUFFER ) ,
                                static_cast<GLenum    >( BufferBindTarget::COPY_WRITE_BUFFER) ,
                                static_cast<GLintptr  >( read_offset ) ,
                                static_cast<GLintptr  >( write_offset) ,
                                static_cast<GLsizeiptr>( size       ) );

            Unbind( BufferBindTarget::COPY_WRITE_BUFFER );
            Unbind( BufferBindTarget::COPY_READ_BUFFER );
        }


        void CopyBufferData( const Buffer & Other )
        {
            CopyBufferData( Other, 0, 0, Other.Size() );
        }

        // reallocates more space for the data
        void Resize(std::size_t size)
        {
            Buffer B;
            B.Reserve(size);

            std::cout << "Resizing Buffer: " << m_ID->first << std::endl;
            std::cout << "Copying " << std::min(size, B.Size() ) << " bytes" << std::endl;

            B.CopyBufferData(*this, 0, 0, std::min(size, Size() ) );
            std::swap( this->m_ID->first, B.m_ID->first );


            SharedData().m_Reserve = B.SharedData().m_Reserve;
            SharedData().m_Offset = std::min( SharedData().m_Reserve, SharedData().m_Offset);

            std::cout << "        New Reserve Size: " << SharedData().m_Reserve << std::endl;
            std::cout << "        New Offset  Size: " << SharedData().m_Offset << std::endl;
            std::cout << "        New Id          : " << m_ID->first << std::endl;

        }

        /**
         * @brief Size
         * @return
         *
         * Gets the total size of the buffer.
         */
        std::size_t Size() const {
            return SharedData().m_Reserve;
            //return m_Reserve;
        }

        /**
         * @brief Offset
         * @return
         *
         * Returns the current offset of the buffer. The offset is where new bytes will
         * be appended to. It indicates the start of unused data.
         */
        std::size_t Offset() const {
            return SharedData().m_Offset;
            //return m_Offset;
        }

        /**
         * @brief Clear
         *
         * Clears the information about the buffer's size. This does not remove the data from the
         * GPU. Instead, this only resets where new data will be written to.
         */
        void Clear()
        {
            //m_Offset = 0;
            SharedData().m_Offset = 0;
        }

        //================================================

        /**
         * @brief Reserve
         * @param size_in_bytes - number of bytes to allocate
         * @param usage
         *
         * Reserves data on the GPU for storing raw data.
         */
        void Reserve(std::size_t size_in_bytes, BufferUsage usage = BufferUsage::STATIC_DRAW)
        {
            auto const targ = BufferBindTarget::COPY_WRITE_BUFFER;

            Regenerate( ); // generates a new handle, releases the old one.
            Bind(  *this , targ);
            glBufferData( static_cast<GLenum>(targ) , size_in_bytes, NULL , static_cast<GLenum>(usage) );
            SharedData().m_Offset    = 0;
            SharedData().m_Reserve   = size_in_bytes;
            std::cout << "Buffer: Reserved Size: " << size_in_bytes << std::endl;
        }


        /**
         * @brief CopyData
         * @param data   - pointer to the data
         * @param size   - size of the data
         * @param offset - offset from the start of the buffer to write to
         *
         * Copies raw data directly into the buffer at a particular offset
         */
        void CopyData(const GLvoid * data, std::size_t size, std::size_t offset)
        {
            auto const targ = BufferBindTarget::COPY_WRITE_BUFFER;

            if( !(*this) )  // If the buffere is not allocated,
            {
                Reserve( size+offset );
            }

            Bind(  *this , targ);

            if( size + offset > SharedData().m_Reserve )
                throw std::runtime_error("Buffer is too small to hold all the data");

            glBufferSubData( static_cast<GLenum>(targ), offset, size, data);

            if( size+offset > SharedData().m_Offset )
            {
                SharedData().m_Offset = size+offset;
            }
            Unbind(targ);
        }


        /**
         * @brief CopyData
         * @param V
         * @param offset
         *
         * Copies a vector data into the buffer.
         */
        template<typename VertexType>
        std::size_t CopyData(const std::vector<VertexType> & V, std::size_t offset = 0)
        {
            CopyData( (const GLvoid *)&V[0], sizeof(VertexType)*V.size(), offset);
            return sizeof(VertexType)*V.size();
        }

        /**
         * @brief Append
         * @param V - a vector of vertices.
         * @return the current offset of the data that was placed into the buffer
         *
         * Copies all the bytes in the input vector and appends it to the end
         * of the buffer, returns the offset in the buffer at which the data was
         * stored.
         *
         * If there is not enough room to hold the data, an exception will be thrown.
         */
        template<typename VertexType>
        std::size_t Append(const std::vector<VertexType> & V)
        {
            auto offset = SharedData().m_Offset;
            CopyData(V, offset);
            return offset;
        }

        /**
         * @brief Append
         * @param V - reference to some data structure, sizeof(V) must return a valid value
         * @return the current offset of the data that was placed into the buffer
         *
         * Copies all the bytes in the input vector and appends it to the end
         * of the buffer, returns the offset in the buffer at which the data was
         * stored.
         *
         * If there is not enough room to hold the data, an exception will be thrown.
         */
        template<typename VertexType>
        std::size_t Append(const VertexType & V)
        {
            auto offset = SharedData().m_Offset;
            CopyData(&V, offset, 0);
            return offset;
        }

        /**
         * @brief operator <<
         * @param V
         * @return the current offset of the data that was placed into the buffer
         *
         * Same as Append()
         */
        template<typename VertexType>
        inline std::size_t operator << (const std::vector<VertexType> & V)
        {
            return Append(V);
        }

        template<typename VertexType>
        inline std::size_t operator << (const VertexType & V)
        {
            return Append(V);
        }

        template<typename VertexType>
        inline std::size_t operator () (const VertexType & V, std::size_t offset=0)
        {
            CopyData(&V, sizeof(V) , offset);
            return sizeof(V);
        }

        template<typename VertexType>
        inline std::size_t operator () (const std::vector<VertexType> & V,  std::size_t offset=0)
        {
            CopyData( V, 0, offset);
            return sizeof(V)*V.size();
        }

//private:
//        std::size_t m_Offset  = 0;
//        std::size_t m_Reserve = 0;

};



}}

#endif
