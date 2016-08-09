#ifndef EXP_ARRAY_BUFFER
#define EXP_ARRAY_BUFFER


#include "buffer_base.h"


namespace gla { namespace experimental
{


class Array_Buffer : public Buffer<BufferBindTarget::ARRAY_BUFFER>
{
    public:
        template<typename VertexData>
        Array_Buffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(data, usage)
        {
        }

        template<bool BindFirst=true>
        void Draw( Primitave p, std::size_t first, std::size_t NumberOfPrimitaves)
        {
            if(BindFirst) Bind();

            glDrawArrays( static_cast<GLenum>(p),  static_cast<GLint>(first),  static_cast<GLsizei>(NumberOfPrimitaves) );
        }
};




}}

#endif
