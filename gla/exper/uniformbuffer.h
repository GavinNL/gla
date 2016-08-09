#ifndef EXP_UNIFORM_BUFER_ARRAY
#define EXP_UNIFORM_BUFER_ARRAY

#include <gla/exper/buffer_base.h>

namespace gla { namespace experimental
{

    class UniformBuffer : public Buffer<BufferBindTarget::UNIFORM_BUFFER>
    {
        public:


             template<typename ProperlyAlignedStructType>
             void operator << (const ProperlyAlignedStructType & structure)
             {
                 Bind();
                 Buffer::CopyData(structure);
             }

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

} }

#endif
