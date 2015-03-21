#ifndef GLRE_GPUTEXTUREARRAY_H
#define GLRE_GPUTEXTUREARRAY_H

#include <glre/global.h>

namespace glre {

class Texture;

//===========================================================================
// GPUTextureArray
//   - an array of 2D textures stored on the GPU.
//===========================================================================
class GPUTextureArray
{

    GPUTextureArray();
    GPUTextureArray(uint width, uint height, uint depth);
    GPUTextureArray(uvec3 size);
    ~GPUTextureArray();

    void create(uvec2 size, unsigned int depth, int MipMapCount);

    inline void bind() { glBindTexture(GL_TEXTURE_2D_ARRAY, mID); }

    void SetLayer( const Texture & T, uint Layer, const uvec2 & pOffset=uvec2(0,0) );

    void clear()
    {
        if(mID)   glDeleteTextures(1, &mID);

        mSize          = {0,0};
        mMipLevelCount =0;
    };

    inline void setActiveTexture(unsigned int unit=0)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mID);
    }

    inline GLuint getID() const { return mID;   };
    inline uvec3  size()  const { return mSize; };

    private:
        GLuint  mID;
        uvec3   mSize;
        GLsizei mMipLevelCount;
};

};

#endif // GPUTEXTUREARRAY_H
