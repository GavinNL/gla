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

#ifndef GLA_DRAW_H
#define GLA_DRAW_H

#include<gla/core/types.h>

namespace gla
{


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



#endif
