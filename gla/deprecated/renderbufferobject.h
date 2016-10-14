#ifndef GLA_RENDERBUFFEROBJECT_H
#define GLA_RENDERBUFFEROBJECT_H


#include <gla/types.h>
#include <gla/texture.h>
#include <vector>

namespace gla
{

enum class RBOFormat : GLuint
{
    RGBA4             = GL_RGBA4 ,
    RGB565            = GL_RGB565,
    RGB5_A1           = GL_RGB5_A1,
    DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
    STENCIL_INDEX8    = GL_STENCIL_INDEX8,
    DEPTH24_STENCIL8  = GL_DEPTH24_STENCIL8
};

struct RenderBufferHandler
{
    inline static void Create  (GLuint & h) { glGenRenderbuffers(1, &h); }
    inline static void Release (GLuint & h) { glDeleteRenderbuffers(1, &h); }
    inline static void Bind    (GLuint & h) { glBindRenderbuffer(GL_RENDERBUFFER,    h); }
    inline static void Unbind  (GLuint & h) { glBindRenderbuffer(GL_RENDERBUFFER,    0);  }
};

struct RenderBufferInfo
{
    RBOFormat format;
    uvec2     size;
};

class RenderBufferObject
{
public:

    using HandleType = gla::Handle<GLuint, RenderBufferHandler ,RenderBufferInfo>;

    HandleType   m_Handle;


    inline void Bind()    { m_Handle.Bind();    }
    inline void Unbind()  { m_Handle.Unbind();  }
    inline void Release() { m_Handle.Release(); }



    RenderBufferObject()
    {

    }

    RenderBufferObject(const uvec2 & size, RBOFormat format)
    {
        Create(size, format);
    }

    RenderBufferObject & Create(const uvec2 & size, RBOFormat format)
    {
        m_Handle.Create();
        m_Handle.Bind();

        glRenderbufferStorage(GL_RENDERBUFFER, (GLuint)format, size.x, size.y);

        m_Handle.__GetInfo().format = format;
        m_Handle.__GetInfo().size   = size;
        m_Handle.Unbind();
        return *this;
    }
};

using GPURenderBufferObject = RenderBufferObject;




}

#endif // RENDERBUFFEROBJECT

