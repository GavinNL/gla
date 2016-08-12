#ifndef EXP_FRAMEBUFFER_
#define EXP_FRAMEBUFFER_

#include <gla/exper/handle.h>
#include <gla/exper/sampler2d.h>


namespace gla { namespace experimental
{

struct GenFrameBuff
{
    void operator()(GLuint & x)
    {
        glGenFramebuffers(1, &x);
        std::cout << "Frame Buffer Generated: " << x << std::endl;
    }
};

struct DestFrameBuff
{
    void operator()(GLuint & x)
    {
        std::cout << "Destroying Frame Buffer: " << x << std::endl;
        glDeleteFramebuffers( 1, &x );
        x = 0;
    }
};



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
};


class FrameBuffer : public BaseHandle<GLuint, GenFrameBuff, DestFrameBuff>
{

public:

    void Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,  Get());
    }

    void UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,  0);
    }

    void Attach(const Sampler2D & Tex,  FrameBufferAttachment attachment);
    void Use( std::vector<FrameBufferAttachment> attach);

    /**
     * @brief GenDepthTexture
     * @param size
     * @return
     *
     * Generates a depth texture that can be used with FrameBuffers
     *
     */
    static Sampler2D CreateBufferTexture_Depth16F( const glm::uvec2 & size)
    {
        gla::experimental::Sampler2D Depth( size , gla::experimental::SamplerFormat::DEPTH_COMPONENT24,  gla::experimental::ImageFormat::DEPTH_COMPONENT, gla::experimental::DataType::FLOAT);
        Depth.SetFilter( gla::experimental::SamplerFilter::NEAREST , gla::experimental::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_RGB( const glm::uvec2 & size)
    {
        gla::experimental::Sampler2D Colours( size );
        Colours.SetFilter( gla::experimental::SamplerFilter::NEAREST , gla::experimental::SamplerFilter::NEAREST );
        return Colours;
    }

    static Sampler2D CreateBufferTexture_RGBA( const glm::uvec2 & size)
    {
        gla::experimental::Sampler2D Colours( size , gla::experimental::SamplerFormat::RGBA,  gla::experimental::ImageFormat::RGBA, gla::experimental::DataType::UNSIGNED_BYTE);
        Colours.SetFilter( gla::experimental::SamplerFilter::NEAREST , gla::experimental::SamplerFilter::NEAREST );
        return Colours;
    }

    static Sampler2D CreateBufferTexture_Vec3_16f( const glm::uvec2 & size)
    {
        gla::experimental::Sampler2D Depth( size , gla::experimental::SamplerFormat::RGB16F,  gla::experimental::ImageFormat::RGB, gla::experimental::DataType::FLOAT);
        Depth.SetFilter( gla::experimental::SamplerFilter::NEAREST , gla::experimental::SamplerFilter::NEAREST );
        return Depth;
    }

    static Sampler2D CreateBufferTexture_Vec4_16f( const glm::uvec2 & size)
    {
        gla::experimental::Sampler2D Depth( size , gla::experimental::SamplerFormat::RGBA16F,  gla::experimental::ImageFormat::RGBA, gla::experimental::DataType::FLOAT);
        Depth.SetFilter( gla::experimental::SamplerFilter::NEAREST , gla::experimental::SamplerFilter::NEAREST );
        return Depth;
    }
};



inline void FrameBuffer::Use( std::vector<FrameBufferAttachment> attach)
{
    //const GLenum * p = static_cast<const GLenum*>( attach.data() );
    Bind();
    const void * p = attach.data();
    glDrawBuffers( attach.size(),  static_cast<const GLenum*>( p ) );
}

inline void FrameBuffer::Attach( const Sampler2D & texture, FrameBufferAttachment attachment)
{
    Bind();
    //texture.Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachment), GL_TEXTURE_2D, texture.Get(), 0);
}

} }



#endif
