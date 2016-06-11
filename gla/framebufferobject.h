#ifndef GLA_FRAMEBUFFEROBJECT_H
#define GLA_FRAMEBUFFEROBJECT_H

#include <gla/types.h>
#include <gla/texture.h>
#include <gla/renderbufferobject.h>
#include <vector>

namespace gla {




enum class FBOAttachment
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
    DEPTH   = GL_DEPTH_ATTACHMENT,
    STENCIL = GL_STENCIL_ATTACHMENT
};


enum class FBOColourFormat
{
//    R8U              =   GL_R8UI,          // These ones dont work
//    R16U             =   GL_R16UI,         // These ones dont work
//    R32U             =   GL_R32UI,         // These ones dont work
//    RG8U             =   GL_RG8UI,         // These ones dont work
//    RG16U            =   GL_RG16UI,        // These ones dont work
//    RG32U            =   GL_RG32UI,        // These ones dont work
//    RGB8U            =   GL_RGB8UI,        // These ones dont work
//    RGB16U           =   GL_RGB16UI,       // These ones dont work
//    RGB32U           =   GL_RGB32UI,       // These ones dont work
//    RGBA8U            =   GL_RGBA8UI,      // These ones dont work
//    RGBA16U           =   GL_RGBA16UI,     // These ones dont work
//    RGBA32U           =   GL_RGBA32UI,     // These ones dont work
    R16F             =   GL_R16F,
    R32F             =   GL_R32F,

    RG16F            =   GL_RG16F,
    RG32F            =   GL_RG32F,

    RGB16F           =   GL_RGB16F,
    RGB32F           =   GL_RGB32F,


    RGBA16F           =   GL_RGBA16F,
    RGBA32F           =   GL_RGBA32F,

    RGBA              =   GL_RGBA,
    RGB               =   GL_RGB,
    RG                =   GL_RG,
    RED               =   GL_RED

};

struct FrameBufferObjectInfo
{
    struct TexAttachment
    {
        FBOAttachment type;
        GPUTexture    tex;
    };

    std::vector<TexAttachment> mAttachments;

    std::map<FBOAttachment, GPUTexture> mAttachment;

};

struct FrameBufferHandler
{
    inline static void Create  (GLuint & h) { glGenFramebuffers(1, &h); }
    inline static void Release (GLuint & h) { glDeleteFramebuffers(1, &h); }
    inline static void Bind    (GLuint & h) { glBindFramebuffer(GL_FRAMEBUFFER,  h); }
    inline static void Unbind  (GLuint & h) { glBindFramebuffer(GL_FRAMEBUFFER,  0);  }
};


class FrameBufferObject
{
    public:
        using HandleType = gla::Handle<GLuint, FrameBufferHandler ,FrameBufferObjectInfo>;

        HandleType   m_Handle;

    public:

        void Bind()     { m_Handle.Bind();    }
        void Unbind()   { m_Handle.Unbind();  }
        void Release()  { m_Handle.Release(); }

        void AttachBuffer(FBOAttachment attch ,   GPUTexture tex)
        {
            if( m_Handle.GetID() == 0 ) Create();

            auto & A = m_Handle.__GetInfo().mAttachments;

            A.push_back( {attch, tex} );

        }


        void CreateColourAttachment(unsigned int Number, const uvec2 & size, FBOColourFormat format)
        {
            if( Number > 15) return;
            if( m_Handle.GetID() == 0) Create();

            auto & A = m_Handle.__GetInfo().mAttachment;

            TexFormat T;
            DataType typ;
            switch( format )
            {
                //case FBOColourFormat::R8U    : T = TexFormat::RED; typ = DataType::UNSIGNED_BYTE; break;
                //case FBOColourFormat::R16U   : T = TexFormat::RED; typ = DataType::UNSIGNED_BYTE; break;
                //case FBOColourFormat::R32U   : T = TexFormat::RED; typ = DataType::UNSIGNED_BYTE; break;
                case FBOColourFormat::R16F   : T = TexFormat::RED; typ = DataType::FLOAT; break;
                case FBOColourFormat::R32F   : T = TexFormat::RED; typ = DataType::FLOAT; break;
                //case FBOColourFormat::RG8U   : T = TexFormat::RG; typ = DataType::UNSIGNED_BYTE;break;
                //case FBOColourFormat::RG16U  : T = TexFormat::RG; typ = DataType::UNSIGNED_BYTE;break;
                //case FBOColourFormat::RG32U  : T = TexFormat::RG; typ = DataType::UNSIGNED_BYTE;break;
                case FBOColourFormat::RG16F  : T = TexFormat::RG; typ = DataType::FLOAT; break;
                case FBOColourFormat::RG32F  : T = TexFormat::RG; typ = DataType::FLOAT; break;
                //case FBOColourFormat::RGB8U  : T = TexFormat::RGB; typ = DataType::UNSIGNED_BYTE;break;
                //case FBOColourFormat::RGB16U : T = TexFormat::RGB; typ = DataType::UNSIGNED_BYTE;break;
                //case FBOColourFormat::RGB32U : T = TexFormat::RGB; typ = DataType::UNSIGNED_BYTE;break;
                case FBOColourFormat::RGB16F : T = TexFormat::RGB; typ = DataType::FLOAT; break;
                case FBOColourFormat::RGB32F : T = TexFormat::RGB; typ = DataType::FLOAT; break;
                //case FBOColourFormat::RGBA8U : T = TexFormat::RGBA; typ = DataType::UNSIGNED_BYTE; break;
                //case FBOColourFormat::RGBA16U: T = TexFormat::RGBA; typ = DataType::UNSIGNED_BYTE; break;
                //case FBOColourFormat::RGBA32U: T = TexFormat::RGBA; typ = DataType::UNSIGNED_BYTE; break;
                case FBOColourFormat::RGBA16F: T = TexFormat::RGBA; typ = DataType::FLOAT; break;
                case FBOColourFormat::RGBA32F: T = TexFormat::RGBA; typ = DataType::FLOAT; break;
                case FBOColourFormat::RGBA   : T = TexFormat::RGBA; typ = DataType::UNSIGNED_BYTE;break;
                case FBOColourFormat::RGB    : T = TexFormat::RGB;  typ = DataType::UNSIGNED_BYTE;break;
                case FBOColourFormat::RG     : T = TexFormat::RG ;  typ = DataType::UNSIGNED_BYTE;break;
                case FBOColourFormat::RED    : T = TexFormat::RED ;  typ = DataType::UNSIGNED_BYTE;break;
                default:
            {
                GLA_DOUT  << "FBO ERROR!!!!" << std::endl;
                return;
            }
            }

            A[ (FBOAttachment)((GLuint)FBOAttachment::COLOR0 + Number) ] = GPUTexture( size, false, (TexInternalFormat)format, T, DataType::UNSIGNED_BYTE);
            A[ (FBOAttachment)((GLuint)FBOAttachment::COLOR0 + Number) ].SetFilter(TexFilter::NEAREST, TexFilter::NEAREST);

           // Update();
        }

        void CreateDepthAttachment(const uvec2 & size)
        {
            if( m_Handle.GetID() == 0) Create();

            auto & A = m_Handle.__GetInfo().mAttachment;

            A[ FBOAttachment::DEPTH ] = GPUTexture( size, false, TexInternalFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, DataType::FLOAT);
            A[ FBOAttachment::DEPTH ].SetFilter(TexFilter::NEAREST, TexFilter::NEAREST);

           // Update();
        }

        GPUTexture GetAttachment( FBOAttachment attch)
        {
            auto & A = m_Handle.__GetInfo().mAttachment;

            auto f = A.find( attch);

            if( f == A.end() )
            {
                return GPUTexture();
            }

            return f->second;
        }

        void Create()
        {
            GLA_DOUT  << "Creating FrameBuffer" << std::endl;
            m_Handle.Release();
            m_Handle.Create();
            m_Handle.Unbind();
        }


        void Update( )
        {
            if( m_Handle.GetID() == 0) Create();

            m_Handle.Bind();

            auto & A = m_Handle.__GetInfo().mAttachment;

            //GLuint attachments[A.size()];
            std::vector<GLuint> attachments;
            int i=0;
            for(auto & a : A)
            {
                if( (GLuint)a.first != (GLuint)FBOAttachment::DEPTH)
                    attachments.push_back( (GLuint)a.first );

//                GLA_DOUT  << attachments[i] << " ::: " << a.second.m_Handle.GetID()  << std::endl;

                glFramebufferTexture2D(GL_FRAMEBUFFER, (GLuint)a.first, GL_TEXTURE_2D, a.second.m_Handle.GetID(), 0);
            }

            glDrawBuffers( attachments.size() , &attachments[0] );

            if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                throw gla::GLA_EXCEPTION("Framebuffer status was not complete.");
            }
        }

//        FrameBufferObject& AttachRenderBuffer( RenderBufferObject & rbo, FBOAttachment attch=FBOAttachment::COLOR_ATTACHMENT0);

};



}
#endif // FRAMEBUFFEROBJECT_H
