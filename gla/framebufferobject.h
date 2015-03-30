#ifndef GLA_FRAMEBUFFEROBJECT_H
#define GLA_FRAMEBUFFEROBJECT_H

#include <gla/types.h>
#include <gla/texture.h>

namespace gla {


class FrameBufferObject
{
    public:
        FrameBufferObject();
        ~FrameBufferObject();


        /**
         * @brief create Creates a framebuffer
         * @param size
         *
         * Creates a FrameBufferObject with colour component and depth component.
         */
        void create(const uvec2 & size);

        void attachTexture( GPUTexture & tex, int attachmentNumber=0);

        inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER, mID); };
        inline void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

        GLuint getID() const { return mID; }

        inline uvec2 size();
        GPUTexture & getRenderTexture() { return mGPUTexture; }



        GLuint     mID;
        GPUTexture mGPUTexture;
        GLuint     mDepthRenderBuffer;



};

}
#endif // FRAMEBUFFEROBJECT_H
