#ifndef GLA_FRAMEBUFFEROBJECT_H
#define GLA_FRAMEBUFFEROBJECT_H

#include <gla/types.h>
#include <gla/texture.h>
#include <gla/renderbufferobject.h>
#include <vector>

namespace gla {




enum class FBOAttachment
{
    COLOR_ATTACHMENT0  = GL_COLOR_ATTACHMENT0,
    COLOR_ATTACHMENT1  = GL_COLOR_ATTACHMENT1,
    COLOR_ATTACHMENT2  = GL_COLOR_ATTACHMENT2,
    COLOR_ATTACHMENT3  = GL_COLOR_ATTACHMENT3,
    COLOR_ATTACHMENT4  = GL_COLOR_ATTACHMENT4,
    COLOR_ATTACHMENT5  = GL_COLOR_ATTACHMENT5,
    COLOR_ATTACHMENT6  = GL_COLOR_ATTACHMENT6,
    COLOR_ATTACHMENT7  = GL_COLOR_ATTACHMENT7,
    COLOR_ATTACHMENT8  = GL_COLOR_ATTACHMENT8,
    COLOR_ATTACHMENT9  = GL_COLOR_ATTACHMENT9,
    COLOR_ATTACHMENT10 = GL_COLOR_ATTACHMENT10,
    COLOR_ATTACHMENT11 = GL_COLOR_ATTACHMENT11,
    COLOR_ATTACHMENT12 = GL_COLOR_ATTACHMENT12,
    COLOR_ATTACHMENT13 = GL_COLOR_ATTACHMENT13,
    COLOR_ATTACHMENT14 = GL_COLOR_ATTACHMENT14,
    COLOR_ATTACHMENT15 = GL_COLOR_ATTACHMENT15,
    DEPTH_ATTACHMENT   = GL_DEPTH_ATTACHMENT,
    STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT
};

struct FrameBufferObjectInfo
{
    std::vector<FBOAttachment> mBuffers;
};

class FrameBufferObject
{
    public:
        FrameBufferObject()
        {
        }

        ~FrameBufferObject(){};

        FrameBufferObject& Create( );

        /**
         * @brief create Creates a framebuffer
         * @param size
         *
         * Creates a FrameBufferObject with colour component and depth component.
         */
       // void create(const uvec2 & size);

        FrameBufferObject& AttachTexture(      GPUTexture & tex, FBOAttachment attch=FBOAttachment::COLOR_ATTACHMENT0);
        FrameBufferObject& AttachRenderBuffer( RenderBufferObject & rbo, FBOAttachment attch=FBOAttachment::COLOR_ATTACHMENT0);

        inline void         bind()   { glBindFramebuffer(GL_FRAMEBUFFER, mID); }
        inline void         unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

        inline GLuint       getID() const { return mID; }
        //inline uvec2        size(){ return mGPUTexture.size(); }
        //inline GPUTexture & getRenderTexture() { return mGPUTexture; }

        GLuint     mID;
        //GPUTexture mGPUTexture;
        //GLuint     mDepthRenderBuffer;

};

inline FrameBufferObject& FrameBufferObject::Create()
{
    glGenFramebuffers(1,             &mID);
    glBindFramebuffer(GL_FRAMEBUFFER, mID);
}

inline FrameBufferObject& FrameBufferObject::AttachRenderBuffer( RenderBufferObject & rbo, FBOAttachment attch)
{
    if( mID == 0 ) return *this;

    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLuint)attch, GL_RENDERBUFFER, rbo.m_ID);

    return *this;
}


inline FrameBufferObject& FrameBufferObject::AttachTexture( GPUTexture & tex, FBOAttachment attch)
{
    if( mID == 0) return *this;

    bind();
    glFramebufferTexture(GL_FRAMEBUFFER, static_cast<unsigned int>(attch), tex.m_Handle.GetID(), 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw gla::GLA_EXCEPTION("Framebuffer status was not complete.");
    }

    return *this;
}

/*
inline void FrameBufferObject::create(const uvec2 & size)
{

    // Create the framebuffer and bind it as the active one
    glGenFramebuffers(1,             &mID);
    glBindFramebuffer(GL_FRAMEBUFFER, mID);



    //=================Create the texture=============================
    glGenTextures(1,            &mGPUTexture.mTextureID);
    glBindTexture(GL_TEXTURE_2D, mGPUTexture.mTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, size.x, size.y, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    mGPUTexture.mDim = size;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //=================================================================


    //=================Create depth buffer=============================
    glGenRenderbuffers(1, &mDepthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderBuffer);
    //=================================================================


    // set the render texture as our colour attachment
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mGPUTexture.mTextureID, 0);


    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw gla::GLA_EXCEPTION("FrameBufferObject did not get created fully.\n");
    }
    std::cout << "FrameBufferObject created: " << mID << ", " << mGPUTexture.mTextureID << " , " << mDepthRenderBuffer << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, mID);
}

*/

}
#endif // FRAMEBUFFEROBJECT_H
