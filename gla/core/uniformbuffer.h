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

#ifndef EXP_UNIFORM_BUFER_ARRAY
#define EXP_UNIFORM_BUFER_ARRAY

#include "bufferbase.h"

namespace gla {


    class UniformBuffer : public Buffer
    {
        public:
             static BufferBindTarget const BindTarget = BufferBindTarget::UNIFORM_BUFFER;

             using Buffer::Buffer;

             void Bind() const
             {
                 Buffer::Bind( *this, BindTarget);
             }
             void Unbind() const
             {
                 Buffer::Unbind(  BindTarget);
             }

           // UniformBuffer() : Buffer(){}
           // UniformBuffer( std::size_t size ) : Buffer(size){}

            template<bool bindfirst=true>
            inline void SetBindPoint(GLuint BindPoint)
            {
                if(bindfirst) Bind();
                glBindBufferBase(GL_UNIFORM_BUFFER, BindPoint, Get() );
            }

            // Static Quering functions
            static GLuint Get_MaxUniformBufferBindings()
            {
                static GLint x = 0;
                if( x ) return x;
                glGetIntegerv (GL_MAX_UNIFORM_BUFFER_BINDINGS, &x);
                return x;
            }

            static GLuint Get_MaxUniformBlockSize()
            {
                static GLint x = 0;
                if( x ) return x;
                glGetIntegerv (GL_MAX_UNIFORM_BLOCK_SIZE, &x);
                return x;
            }

            static GLuint Get_MaxCombinedUniformBlocks()
            {
                static GLint x = 0;
                if( x ) return x;
                glGetIntegerv (GL_MAX_COMBINED_UNIFORM_BLOCKS, &x);
                return x;
            }

    };

}

#endif
