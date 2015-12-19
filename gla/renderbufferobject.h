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

class RenderBufferObject
{
public:
    struct Info
    {
        RBOFormat format;
        uvec2     size;
    };


    RenderBufferObject() {}

    RenderBufferObject & Create(const uvec2 & size , RBOFormat format );
    RenderBufferObject & Destroy();

    void bind()   { glBindRenderbuffer(GL_RENDERBUFFER, m_ID); }
    void unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }



    GLuint m_ID = 0;
};


inline RenderBufferObject & RenderBufferObject::Create(const uvec2 & size, RBOFormat format)
{

    glGenRenderbuffers(1, &m_ID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
    glRenderbufferStorage(GL_RENDERBUFFER, (GLuint)format, size.x, size.y);

    return *this;
}

inline RenderBufferObject & RenderBufferObject::Destroy()
{
    glDeleteRenderbuffers(1, &m_ID);
    return *this;
}

}

#endif // RENDERBUFFEROBJECT

