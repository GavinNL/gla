#ifndef EXP_VERTEX_ARRAY
#define EXP_VERTEX_ARRAY

#include <gla/glad.h>
#include <gla/exper/handle.h>
#include <gla/exper/array_buffer.h>

namespace gla { namespace experimental {

struct GenVertexArray
{
    void operator()(GLuint & h) { glGenVertexArrays(1, &h); }
};


struct DestVertexArray
{
    void operator()(GLuint & h) { glDeleteVertexArrays(1, &h); }
};


class VertexArray : public BaseHandle<GLuint, GenVertexArray, DestVertexArray>
{
    public:
        void Bind  () { glBindVertexArray( Get() ); }
        void Unbind() { glBindVertexArray(  0    ); }

        template <typename... GLM_Vec_Types>
        void Attach(const Element_Array_Buffer & IndexArray, const Array_Buffer & Attribute )
        {
            if( !(*this) )
                Generate();


            Bind();
                IndexArray.Bind();
                m_Data = IndexArray.m_Data;

                Attribute.Bind();
                Attribute.EnableAttributes< GLM_Vec_Types... >();

                //#error dont use this yet.
            Unbind();

        }

        template <typename... GLM_Vec_Types>
        void Attach(const Array_Buffer & Attribute )
        {
            if( !(*this) )
                Generate();

            m_Data = DataType::UNKNOWN;

            Bind();
                Attribute.Bind();
                Attribute.EnableAttributes< GLM_Vec_Types... >();

                //#error dont use this yet.
            Unbind();

        }

        template<typename ...GlaBufferTypes >
        VertexArray()
        {

        }

        template<bool BindFirst=true>
        void Draw( Primitave p, std::size_t NumberOfIndices, std::size_t first=0 )
        {
            if(BindFirst) Bind();

            m_Data==DataType::UNKNOWN ?
                glDrawArrays( static_cast<GLenum>(p),  static_cast<GLint>(first),  static_cast<GLsizei>(NumberOfIndices) )
                      :
                glDrawElements( static_cast<GLenum>(p),
                                static_cast<GLsizei>(NumberOfIndices),
                                static_cast<GLenum>(m_Data),
                                static_cast<char*>(0)+first
                                );
        }

        DataType m_Data = DataType::UNKNOWN;
};


} }

#endif
