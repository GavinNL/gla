#ifndef GLA_DRAW_H
#define GLA_DRAW_H

#include<gla/core/types.h>

namespace gla
{
namespace core
{
    using namespace gla::experimental;

    static void DrawElements( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndex)
    {
        glDrawElements( static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndex);
    }

    static void DrawArrays( Primitave p, std::size_t NumVertices, std::size_t StartVertex)
    {
        glDrawArrays( static_cast<GLenum>(p),
                      static_cast<GLint>(StartVertex),
                      static_cast<GLsizei>(NumVertices) );

    }

    static void DrawElementsInstanced( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndex, std::size_t primcount)
    {
        glDrawElementsInstanced( static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndex,
                        static_cast<GLsizei>(primcount) ) ;
    }

    static void DrawArraysInstanced( Primitave p, std::size_t NumVertices, std::size_t StartVertex, std::size_t primcount)
    {
        glDrawArraysInstanced( static_cast<GLenum>(p),
                      static_cast<GLint>(StartVertex),
                      static_cast<GLsizei>(NumVertices), static_cast<GLsizei>(primcount) );

    }


    static void DrawElementsBaseVertex( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndex, std::size_t BaseVertex)
    {


        glDrawElementsBaseVertex( static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndex,
                        static_cast<GLint>(BaseVertex) );
    }

    static void DrawElementsInstancedBaseVertex( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndex, std::size_t BaseVertex, std::size_t primcount)
    {


        glDrawElementsInstancedBaseVertex( static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndex,
                        static_cast<GLsizei>(primcount),
                        static_cast<GLint>(BaseVertex) );
    }
}

}

#endif
