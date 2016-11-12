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

#ifndef EXP_FRAMEBUFFER_
#define EXP_FRAMEBUFFER_

#include "handle.h"
#include "sampler2d.h"


namespace gla {

struct GenFrameBuff
{
    void operator()(GLuint & x)
    {
        glGenFramebuffers(1, &x);
        GLA_LOGD << "Frame Buffer Generated: " << x << std::endl;
    }
};

struct DestFrameBuff
{
    void operator()(GLuint & x)
    {
        GLA_LOGD << "Destroying Frame Buffer: " << x << std::endl;
        glDeleteFramebuffers( 1, &x );
        x = 0;
    }
};

/*

enum class FrameBufferAttachment : GLenum
{
    COLOR0  = GL_COLOR_ATTACHMENT0,
    COLOR1  = GL_COLOR_ATTACHMENT1,
    COLOR2  = GL_COLOR_ATTACHMENT2,
    COLOR3  = GL_COLOR_ATTACHMENT3,
    COLOR4  = GL_COLOR_ATTACHMENT4,
    COLOR5  = GL_COLOR_ATTACHMENT5,
    COLOR6  = GL_COLOR_ATTACHMENT6,
    COLOR7  = GL_COLOR_ATTACHMENT7,
    COLOR8  = GL_COLOR_ATTACHMENT8,
    COLOR9  = GL_COLOR_ATTACHMENT9,
    COLOR10 = GL_COLOR_ATTACHMENT10,
    COLOR11 = GL_COLOR_ATTACHMENT11,
    COLOR12 = GL_COLOR_ATTACHMENT12,
    COLOR13 = GL_COLOR_ATTACHMENT13,
    COLOR14 = GL_COLOR_ATTACHMENT14,
    COLOR15 = GL_COLOR_ATTACHMENT15,
    COLOR16 = GL_COLOR_ATTACHMENT16,
    COLOR17 = GL_COLOR_ATTACHMENT17,
    COLOR18 = GL_COLOR_ATTACHMENT18,
    COLOR19 = GL_COLOR_ATTACHMENT19,
    COLOR20 = GL_COLOR_ATTACHMENT20,
    COLOR21 = GL_COLOR_ATTACHMENT21,
    COLOR22 = GL_COLOR_ATTACHMENT22,
    COLOR23 = GL_COLOR_ATTACHMENT23,
    COLOR24 = GL_COLOR_ATTACHMENT24,
    COLOR25 = GL_COLOR_ATTACHMENT25,
    COLOR26 = GL_COLOR_ATTACHMENT26,
    COLOR27 = GL_COLOR_ATTACHMENT27,
    COLOR28 = GL_COLOR_ATTACHMENT28,
    COLOR29 = GL_COLOR_ATTACHMENT29,
    COLOR30 = GL_COLOR_ATTACHMENT30,
    COLOR31 = GL_COLOR_ATTACHMENT31,

    DEPTH   = GL_DEPTH_ATTACHMENT,
    STENCIL = GL_STENCIL_ATTACHMENT
};*/


class FrameBuffer : public BaseHandle<GLuint, GenFrameBuff, DestFrameBuff>
{

public:

    enum Attachment : GLenum
    {
        COLOR0  = GL_COLOR_ATTACHMENT0,
        COLOR1  = GL_COLOR_ATTACHMENT1,
        COLOR2  = GL_COLOR_ATTACHMENT2,
        COLOR3  = GL_COLOR_ATTACHMENT3,
        COLOR4  = GL_COLOR_ATTACHMENT4,
        COLOR5  = GL_COLOR_ATTACHMENT5,
        COLOR6  = GL_COLOR_ATTACHMENT6,
        COLOR7  = GL_COLOR_ATTACHMENT7,
        COLOR8  = GL_COLOR_ATTACHMENT8,
        COLOR9  = GL_COLOR_ATTACHMENT9,
        COLOR10 = GL_COLOR_ATTACHMENT10,
        COLOR11 = GL_COLOR_ATTACHMENT11,
        COLOR12 = GL_COLOR_ATTACHMENT12,
        COLOR13 = GL_COLOR_ATTACHMENT13,
        COLOR14 = GL_COLOR_ATTACHMENT14,
        COLOR15 = GL_COLOR_ATTACHMENT15,
        COLOR16 = GL_COLOR_ATTACHMENT16,
        COLOR17 = GL_COLOR_ATTACHMENT17,
        COLOR18 = GL_COLOR_ATTACHMENT18,
        COLOR19 = GL_COLOR_ATTACHMENT19,
        COLOR20 = GL_COLOR_ATTACHMENT20,
        COLOR21 = GL_COLOR_ATTACHMENT21,
        COLOR22 = GL_COLOR_ATTACHMENT22,
        COLOR23 = GL_COLOR_ATTACHMENT23,
        COLOR24 = GL_COLOR_ATTACHMENT24,
        COLOR25 = GL_COLOR_ATTACHMENT25,
        COLOR26 = GL_COLOR_ATTACHMENT26,
        COLOR27 = GL_COLOR_ATTACHMENT27,
        COLOR28 = GL_COLOR_ATTACHMENT28,
        COLOR29 = GL_COLOR_ATTACHMENT29,
        COLOR30 = GL_COLOR_ATTACHMENT30,
        COLOR31 = GL_COLOR_ATTACHMENT31,

        DEPTH   = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT
    };

    enum Status
    {
        COMPLETE                      = GL_FRAMEBUFFER_COMPLETE                      ,// is returned if the specified framebuffer is complete. Otherwise, the return value is determined as follows:
        UNDEFINED                     = GL_FRAMEBUFFER_UNDEFINED                     ,// is returned if the specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist.
        INCOMPLETE_ATTACHMENT         = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT         ,// is returned if any of the framebuffer attachment points are framebuffer incomplete.
        INCOMPLETE_MISSING_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ,// is returned if the framebuffer does not have at least one image attached to it.
        INCOMPLETE_DRAW_BUFFER        = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER        ,// is returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi.
        INCOMPLETE_READ_BUFFER        = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER        ,// is returned if GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.
        UNSUPPORTED                   = GL_FRAMEBUFFER_UNSUPPORTED                   ,// is returned if the combination of internal formats of the attached images violates an implementation-dependent set of restrictions.
        INCOMPLETE_MULTISAMPLE        = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE        ,// is returned if the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.
                                                                                      // is also returned if the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.
        INCOMPLETE_LAYER_TARGETS      = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS      // is returned if any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.
    };

    void Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,  Get());
    }

    void Unbind()
    {
        UnBind();
    }

    void UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,  0);
    }

    void Attach(const Sampler2D & Tex,  FrameBuffer::Attachment attachment);

    void Detach(Attachment attachment);

    void Use( std::vector<FrameBuffer::Attachment> attach);

    Status Check()
    {
        Bind();
        return static_cast<Status>( glCheckFramebufferStatus(GL_FRAMEBUFFER) );
    }

    bool Complete()
    {
        return Check()==COMPLETE;
    }

    /**
     * @brief GenDepthTexture
     * @param size
     * @return
     *
     * Generates a depth texture that can be used with FrameBuffers
     *
     */
    static Sampler2D CreateBufferTexture_Depth( const glm::uvec2 & size)
    {
        gla::Sampler2D Depth( size , gla::SamplerFormat::DEPTH_COMPONENT,  gla::ImageFormat::DEPTH_COMPONENT, gla::DataType::FLOAT);
        Depth.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_Depth16F( const glm::uvec2 & size)
    {
        gla::Sampler2D Depth( size , gla::SamplerFormat::DEPTH_COMPONENT16,  gla::ImageFormat::DEPTH_COMPONENT, gla::DataType::FLOAT);
        Depth.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_Depth24F( const glm::uvec2 & size)
    {
        gla::Sampler2D Depth( size , gla::SamplerFormat::DEPTH_COMPONENT24,  gla::ImageFormat::DEPTH_COMPONENT, gla::DataType::FLOAT);
        Depth.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_RGB( const glm::uvec2 & size)
    {
        gla::Sampler2D Colours( size );
        Colours.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Colours;
    }

    static Sampler2D CreateBufferTexture_RGBA( const glm::uvec2 & size)
    {
        gla::Sampler2D Colours( size , gla::SamplerFormat::RGBA,  gla::ImageFormat::RGBA, gla::DataType::UNSIGNED_BYTE);
        Colours.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Colours;
    }

    static Sampler2D CreateBufferTexture_Vec3_16f( const glm::uvec2 & size)
    {
        gla::Sampler2D Depth( size , gla::SamplerFormat::RGB16F,  gla::ImageFormat::RGB, gla::DataType::FLOAT);
        Depth.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_Vec4_16f( const glm::uvec2 & size)
    {
        gla::Sampler2D Depth( size , gla::SamplerFormat::RGBA16F,  gla::ImageFormat::RGBA, gla::DataType::FLOAT);
        Depth.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_Vec3_32f( const glm::uvec2 & size)
    {
        gla::Sampler2D Depth( size , gla::SamplerFormat::RGB32F,  gla::ImageFormat::RGB, gla::DataType::FLOAT);
        Depth.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_Vec4_32f( const glm::uvec2 & size)
    {
        gla::Sampler2D Depth( size , gla::SamplerFormat::RGBA32F,  gla::ImageFormat::RGBA, gla::DataType::FLOAT);
        Depth.SetFilter( gla::SamplerFilter::NEAREST , gla::SamplerFilter::NEAREST );
        return Depth;
    }

};



inline void FrameBuffer::Use( std::vector<FrameBuffer::Attachment> attach)
{
    //const GLenum * p = static_cast<const GLenum*>( attach.data() );
    Bind();
    const void * p = attach.data();
    glDrawBuffers( static_cast<GLsizei>(attach.size()),  static_cast<const GLenum*>( p ) );
}

inline void FrameBuffer::Attach( const Sampler2D & texture, Attachment attachment)
{
    Bind();
    //texture.Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachment), GL_TEXTURE_2D, texture.Get(), 0);
}

inline void FrameBuffer::Detach( Attachment attachment)
{
    Bind();
    //texture.Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachment), GL_TEXTURE_2D, 0, 0);
}

}



#endif
