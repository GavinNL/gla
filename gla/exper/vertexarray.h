#ifndef EXP_VERTEX_ARRAY
#define EXP_VERTEX_ARRAY

#include <gla/glad.h>
#include <gla/exper/handle.h>

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
};


} }

#endif
