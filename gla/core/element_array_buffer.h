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

#ifndef EXP_ELEMENT_ARRAY_BUFFER
#define EXP_ELEMENT_ARRAY_BUFFER

#include "array_buffer.h"

namespace gla {

/**
 * @brief The ElementArrayBuffer class
 *
 * An element array buffer is used to hold indices for
 */
class ElementArrayBuffer : public Buffer
{
    public:
        static BufferBindTarget const BindTarget = BufferBindTarget::ELEMENT_ARRAY_BUFFER;


        ElementArrayBuffer() : Buffer()
        {

        }

        ElementArrayBuffer( std::size_t size ) : Buffer(size){}

        void Bind() const
        {
            Buffer::Bind( *this, BindTarget);
        }
        void Unbind() const
        {
            Buffer::Unbind(  BindTarget );
        }

        template<typename VertexData>
        ElementArrayBuffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(data, usage)
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
        void Draw( Primitave p, std::size_t NumberOfIndices, std::size_t First_Index=0 )
        {
            if(BindFirst)
                Bind();

            gla::DrawElements( p,
                            NumberOfIndices,
                            m_Data,
                            First_Index
                            );
        }

        template<bool BindFirst=true>
        void DrawInstanced( Primitave p , std::size_t NumberOfIndices , std::size_t primcount, std::size_t First_Index=0 )
        {
            if(BindFirst)
                Bind();

            glDrawElementsInstanced( static_cast<GLenum>(p),
                            static_cast<GLsizei>(NumberOfIndices),
                            static_cast<GLenum>(m_Data),
                            static_cast<char*>(0)+First_Index,
                            static_cast<GLsizei>(primcount)
                            );

        }

        DataType   m_Data;
};

}

#endif
