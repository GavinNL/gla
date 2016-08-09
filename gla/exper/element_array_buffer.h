#ifndef EXP_ELEMENT_ARRAY_BUFFER
#define EXP_ELEMENT_ARRAY_BUFFER

#include "array_buffer.h"

namespace gla { namespace experimental
{

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

}}

#endif
