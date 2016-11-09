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


#ifndef EXP_VERTEX_ARRAY
#define EXP_VERTEX_ARRAY

#include <gla/core/handle.h>
#include <gla/core/elementarraybuffer.h>

namespace gla {

struct GenVertexArray
{
    void operator()(GLuint & h) const { glGenVertexArrays(1, &h);  GLA_LOGV << "VOA generated: " << h << std::endl;}
};


struct DestVertexArray
{
    void operator()(GLuint & h) const { GLA_LOGV << "VOA destroyed: " << h << std::endl; glDeleteVertexArrays(1, &h); }
};

struct VertexArrayInfo
{
    Buffer::SharedHandle elementbuffer;
    Buffer::SharedHandle vertexbuffer;
};

class VertexArray : public BaseHandle<GLuint, GenVertexArray, DestVertexArray,VertexArrayInfo>
{
    public:
        void Bind  () const { glBindVertexArray( Get() ); }
        void Unbind() const { glBindVertexArray(  0    ); }

        VertexArray() {}

        template <typename... GLM_Vec_Types>
        VertexArray(const ArrayBuffer & Attribute, const ElementArrayBuffer & IndexArray, NormalizeFlags Normalize_Flags = NormalizeFlags::none)
        {
            Attach<GLM_Vec_Types...>(Attribute, IndexArray, Normalize_Flags);
        }

        template <typename... GLM_Vec_Types>
        VertexArray(const ArrayBuffer & Attribute, NormalizeFlags Normalize_Flags = NormalizeFlags::none)
        {
            Attach<GLM_Vec_Types...>(Attribute, Normalize_Flags);
        }

        template <typename... GLM_Vec_Types>
        void Attach(const ArrayBuffer & Attribute , const ElementArrayBuffer & IndexArray , NormalizeFlags Normalize_Flags = NormalizeFlags::none)
        {
            if( !(*this) )
                Generate();


            Bind();
                SharedData().elementbuffer = IndexArray.GetSharedHandle();
                SharedData().vertexbuffer  =  Attribute.GetSharedHandle();

                IndexArray.Bind();
                m_Data = IndexArray.m_Data;
                //std::cout << "Vertex Array attached an element buffer: data type: " << int(m_Data) << std::endl;
                Attribute.Bind();
                Attribute.EnableAttributes< GLM_Vec_Types... >( Normalize_Flags._flags);

            Unbind();

            Attribute.Unbind();
            IndexArray.Unbind();

        }

        template <typename... GLM_Vec_Types>
        void Attach(const ArrayBuffer & Attribute , NormalizeFlags Normalize_Flags = NormalizeFlags::none )
        {
            if( !(*this) )
                Generate();

            m_Data = DataType::UNKNOWN;

            Bind();

                SharedData().vertexbuffer  = Attribute.GetSharedHandle();

                Attribute.Bind();
                Attribute.EnableAttributes< GLM_Vec_Types... >( Normalize_Flags );
            Unbind();

            Attribute.Unbind();
        }

        template<bool BindFirst=true>
        void Draw( Primitave p, std::size_t NumberOfIndices,  std::size_t First_Index_To_Draw_From=0 )
        {
            if(BindFirst) Bind();

            m_Data==DataType::UNKNOWN ?
                gla::DrawArrays( p,  NumberOfIndices, First_Index_To_Draw_From  )
                //glDrawArrays( static_cast<GLenum>(p),  static_cast<GLint>(First_Index_To_Draw_From),  static_cast<GLsizei>(NumberOfIndices) )
                      :
                gla::DrawElements( p,NumberOfIndices, m_Data, First_Index_To_Draw_From);
        }

        template<bool BindFirst=true>
        void DrawInstanced( Primitave p, std::size_t NumberOfIndices, std::size_t primcount, std::size_t First_Index_To_Draw_From=0 )
        {
            if(BindFirst) Bind();

            m_Data==DataType::UNKNOWN ?
                gla::DrawArrays( p,  NumberOfIndices, First_Index_To_Draw_From, primcount )
                      :
                gla::DrawElements( p, NumberOfIndices,m_Data,First_Index_To_Draw_From, primcount );
        }

        template<typename ...GLM_Types>
        static VertexArray MakeVAO(  const ArrayBuffer & Vertex, const ElementArrayBuffer & Element, NormalizeFlags Normalize_Flags = NormalizeFlags::none  )
        {
            VertexArray vao;
            vao.Attach<GLM_Types...>(Vertex, Element, Normalize_Flags);
            return vao;
        }

        template<typename ...GLM_Types>
        static VertexArray MakeVAO(  const ArrayBuffer & Vertex, NormalizeFlags Normalize_Flags = NormalizeFlags::none  )
        {
            VertexArray vao;
            vao.Attach<GLM_Types...>(Vertex, Normalize_Flags);
            return vao;
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
                EnableAttributes<GLM_Types...>(Normalize_Flags);
            vao.Unbind();
            return vao;
        }



        void MultiDraw( std::vector<gla::MultiDrawElementsIndirectCommand> & cmd )
        {
            Bind();
            if( m_DataType == DataType::UNKNOWN)
            {
                GLA_LOGD << "Cannot MultiDrawElements because there is no ElementBuffer attached to this VAO" << std::endl;
            }
            else {
                gla::MultiDrawElementsIndirect( gla::Primitave::TRIANGLES , m_DataType , cmd);
            }
        }
};

}

#endif
