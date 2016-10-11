#ifndef EXP_VERTEX_ARRAY
#define EXP_VERTEX_ARRAY

#include <gla/core/handle.h>
#include <gla/core/element_array_buffer.h>

namespace gla { namespace experimental {

struct GenVertexArray
{
    void operator()(GLuint & h) const { glGenVertexArrays(1, &h); }
};


struct DestVertexArray
{
    void operator()(GLuint & h) const { glDeleteVertexArrays(1, &h); }
};


class VertexArray : public BaseHandle<GLuint, GenVertexArray, DestVertexArray>
{
    public:
        void Bind  () { glBindVertexArray( Get() ); }
        void Unbind() { glBindVertexArray(  0    ); }

        VertexArray() {}

        template <typename... GLM_Vec_Types>
        VertexArray(const ElementArrayBuffer & IndexArray, const ArrayBuffer & Attribute)
        {
            Attach<GLM_Vec_Types...>(IndexArray,Attribute);
        }

        template <typename... GLM_Vec_Types>
        VertexArray(const ArrayBuffer & Attribute)
        {
            Attach<GLM_Vec_Types...>(Attribute);
        }

        template <typename... GLM_Vec_Types>
        void Attach(const ElementArrayBuffer & IndexArray, const ArrayBuffer & Attribute )
        {
            if( !(*this) )
                Generate();


            Bind();
                IndexArray.Bind();
                m_Data = IndexArray.m_Data;

                Attribute.Bind();
                Attribute.EnableAttributes< GLM_Vec_Types... >();


            Unbind();

        }

        template <typename... GLM_Vec_Types>
        void Attach(const ArrayBuffer & Attribute )
        {
            if( !(*this) )
                Generate();

            m_Data = DataType::UNKNOWN;

            Bind();
                Attribute.Bind();
                Attribute.EnableAttributes< GLM_Vec_Types... >();
            Unbind();

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



class VertexArray_T : public BaseHandle<GLuint, GenVertexArray, DestVertexArray>
{
    public:
        void Bind  () const { glBindVertexArray( Get() ); }
        void Unbind() const { glBindVertexArray(  0    ); }

        std::size_t m_NumberOfVertices_Or_Indices = 0;
        std::size_t m_StartVertex_Or_Index        = 0;
        DataType    m_DataType                    = DataType::UNKNOWN;

        template<bool BindFirst=true>
        void Draw(Primitave p )
        {
            if(BindFirst) Bind();

            m_DataType == DataType::UNKNOWN ?
                        glDrawArrays( static_cast<GLenum>(p),
                                      static_cast<GLint>(m_StartVertex_Or_Index),
                                      static_cast<GLsizei>(m_NumberOfVertices_Or_Indices) )
                        :
                        glDrawElements( static_cast<GLenum>(p),
                                        static_cast<GLsizei>(m_NumberOfVertices_Or_Indices),
                                        static_cast<GLenum>(m_DataType),
                                        static_cast<char*>(0)+m_StartVertex_Or_Index
                                        );

        }

        template<bool BindFirst=true>
        void Draw(Primitave p, std::size_t NumberOfVertices_Or_Indices, std::size_t  StartVertex_Or_Index=0)
        {
            if(BindFirst) Bind();
            m_DataType == DataType::UNKNOWN ?
                        glDrawArrays( static_cast<GLenum>(p),
                                      static_cast<GLint>(StartVertex_Or_Index),
                                      static_cast<GLsizei>(NumberOfVertices_Or_Indices) )
                        :
                        glDrawElements( static_cast<GLenum>(p),
                                        static_cast<GLsizei>(NumberOfVertices_Or_Indices),
                                        static_cast<GLenum>(m_DataType),
                                        static_cast<char*>(0)+StartVertex_Or_Index
                                        );
        }

        //====================================================================

        template<typename ...GLM_Types>
        static VertexArray_T MakeVAO( const ElementArrayBuffer & Element, const ArrayBuffer & Vertex,  std::uint32_t Normalize_Flags = 0  )
        {
            VertexArray_T vao;
            vao.Generate();
            vao.Bind();

            vao.m_DataType = Element.m_Data;

            vao.Bind();
                Vertex.Bind( );
                Element.Bind( );
                EnableAttributes<GLM_Types...>::Enable(0,0,Normalize_Flags);
            vao.Unbind();
            return vao;
        }

};

} }

#endif
