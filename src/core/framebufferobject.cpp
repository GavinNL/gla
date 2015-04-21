#include "gla/core/framebufferobject.h"
#include <gla/core/exceptions.h>
#include <gla/core/texture.h>

namespace gla {


FrameBufferObject::FrameBufferObject()
{

}

FrameBufferObject::~FrameBufferObject()
{

}

void FrameBufferObject::attachTexture( GPUTexture &tex, int attachmentNumber)
{

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+attachmentNumber, tex.getID(), 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw gla::GLA_EXCEPTION("Framebuffer status was not complete.");
    }
}

uvec2 FrameBufferObject::size()
{
    return mGPUTexture.size();
}

void FrameBufferObject::create(const uvec2 & size)
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



    //=================Create depth buffer=============================
    glGenRenderbuffers(1, &mDepthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderBuffer);


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

}
