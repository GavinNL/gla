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


    /**
     * @brief DrawElements
     * @param p - the type of primative to draw
     * @param NumIndices - the number of indices to use
     * @param IndexType - the index type, must be an integer type: (BYTE,UNSIGNED_BYTE,SHORT,UNSIGNED_SHORT ,INT)
     * @param StartIndex - the start index in the buffer to start drawing from.
     *
     * Draws the currently bound Element_Array_Buffer and the ArrayBuffer
     */
    static void DrawElements( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndexByteLocation)
    {
        glDrawElements( static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndexByteLocation);
    }

    /**
     * @brief DrawElements
     * @param p - the primative to draw
     * @param NumIndices - total number of indices to draw
     * @param IndexType - the index type, must be an integer type (BYTE,UNSIGNED_BYTE,SHORT,UNSIGNED_SHORT ,INT)
     * @param StartIndex - start index in the ElementArrayBuffer to start drawing from
     * @param primcount - total number of times to draw the set
     *
     * Draw the same set of primatives multiple times, each time incrementing the gl_InstanceID variable
     * in the shader.  This is used to draw multiple objects of the same time with a single call.
     */
    static void DrawElements( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndexByteLocation, std::size_t primcount)
    {
        glDrawElementsInstanced( static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndexByteLocation,
                        static_cast<GLsizei>(primcount) ) ;
    }

    /**
     * @brief DrawArrays
     * @param p - the type of primitave to draw
     * @param NumVertices - the total number of vertices to draw
     * @param StartVertex - the start vertex in the buffer to draw from
     *
     * Draws the currently bound ArrayBuffer
     */
    static void DrawArrays( Primitave p, std::size_t NumVertices, std::size_t StartVertex)
    {
        glDrawArrays( static_cast<GLenum>(p),
                      static_cast<GLint>(StartVertex),
                      static_cast<GLsizei>(NumVertices) );

    }

    /**
     * @brief DrawArrays
     * @param p
     * @param NumVertices
     * @param StartVertex
     * @param primcount - number of times to draw the set of vertices
     *
     *
     */
    static void DrawArrays( Primitave p, std::size_t NumVertices, std::size_t StartVertex, std::size_t primcount)
    {

        glDrawArraysInstanced( static_cast<GLenum>(p),
                               static_cast<GLint>(StartVertex),
                               static_cast<GLsizei>(NumVertices),
                               static_cast<GLsizei>(primcount) );

    }


    static void DrawElementsBaseVertex( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndexByteLocation, std::size_t BaseVertex)
    {
        glDrawElementsBaseVertex( static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndexByteLocation,
                        static_cast<GLint>(BaseVertex) );
    }

    static void DrawElementsInstancedBaseVertex( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndexByteLocation, std::size_t BaseVertex, std::size_t primcount)
    {

        glDrawElementsInstancedBaseVertex(
                        static_cast<GLenum>(p),
                        static_cast<GLsizei>(NumIndices),
                        static_cast<GLenum>(IndexType),
                        static_cast<char*>(0)+StartIndexByteLocation,
                        static_cast<GLsizei>(primcount),
                        static_cast<GLint>(BaseVertex) );
    }

    static void DrawElementsInstancedBaseVertexBaseInstance( Primitave p, std::size_t NumIndices, DataType IndexType, std::size_t StartIndexByteLocation, std::size_t BaseVertex, std::size_t primcount, std::size_t baseinstance)
    {

        glDrawElementsInstancedBaseVertexBaseInstance(  static_cast<GLenum>(p),
                                                        static_cast<GLsizei>(NumIndices),
                                                        static_cast<GLenum>(IndexType),
                                                        static_cast<char*>(0)+StartIndexByteLocation,
                                                        static_cast<GLsizei>(primcount),
                                                        static_cast<GLint>(BaseVertex),
                                                        static_cast<GLuint>(baseinstance) );



    }

    struct MultiDrawElementsIndirectCommand
    {
        std::uint32_t  count        = 0; // number of indices to draw
        std::uint32_t  instanceCount= 1; // total number of instances of this object to draw
        std::uint32_t  firstIndex   = 0; // The first index in the element array to draw from
        std::uint32_t  baseVertex   = 0; // the value to add to the index obtained from the element array buffer
        std::uint32_t  baseInstance = 0; // the base instance???
    };

    static void MultiDrawElementsIndirect(Primitave p, DataType IndexType, std::vector<MultiDrawElementsIndirectCommand> & commands)
    {
        glMultiDrawElementsIndirect( static_cast<GLenum>(p),
                                     static_cast<GLenum>(IndexType),
                                     static_cast<const void*>(commands.data()),
                                     commands.size(),
                                     0*sizeof(MultiDrawElementsIndirectCommand)
                                     );
    }

    static void MultiDrawElementsIndirect(Primitave p, DataType IndexType, MultiDrawElementsIndirectCommand * commands, std::size_t num_commands)
    {
        glMultiDrawElementsIndirect( static_cast<GLenum>(p),
                                     static_cast<GLenum>(IndexType),
                                     static_cast<const void*>(commands),
                                     num_commands,
                                     0*sizeof(MultiDrawElementsIndirectCommand)
                                     );
    }


}



#endif
