#ifndef GLA_GPUTEXTUREARRAY_H
#define GLA_GPUTEXTUREARRAY_H

#include <gla/global.h>

namespace gla {

//===========================================================================
// GPUTextureArray
//   - an array of 2D textures stored on the GPU.
//===========================================================================
class GPUTextureArray
{

public:
    GPUTextureArray();
    GPUTextureArray(uint width, uint height, uint depth);
    GPUTextureArray(uvec3 size);
    ~GPUTextureArray();

    /**
     * @brief create Creates the texture array on the GPU
     * @param size the width and height dimensions of the textures
     * @param depth the number of textures to allocate space for
     * @param MipMapCount the mipmap level, default is 1
     */
    void create(uvec2 size, unsigned int depth, int MipMapCount=1);

    /**
     * @brief SetLayer - Copies a CPU texture into a particular depth in the Texture Array
     * @param T - CPU texture to copy. This must be the same dimensions as the TextureArray
     * @param Layer - the layer number to copy the texture into
     * @param pOffset - an offset parameter.
     */
//    void SetLayer( const Texture & T, uint Layer, const uvec2 & pOffset=uvec2(0,0) );

    void SetLayer(const TextureRGBA & T, uint Layer, const uvec2 & pOffset=uvec2(0,0))
    {
        bind();

        if( Layer >= mSize.z)
        {
            throw gla::GLA_EXCEPTION("ERROR: Layer number is larger than the allocated size of the TextureArray.");
        }

        if( T.size().x + pOffset.x > mSize.x || T.size().y + pOffset.y > mSize.y )
        {
            throw gla::GLA_EXCEPTION("ERROR: The texture dimensions do not match");
        }

        glTexSubImage3D(  GL_TEXTURE_2D_ARRAY,
                          0,                // level
                          pOffset.x,        // x-offset
                          pOffset.y,        // y-offset
                          Layer,                // z-offset
                          T.size().x,       // width
                          T.size().y,       // height
                          1,            // depth
                          GL_RGBA,          // format
                          GL_UNSIGNED_BYTE, // type
                          T.getRawData()    // image data
                       );
    }

    inline void bind() { glBindTexture(GL_TEXTURE_2D_ARRAY, mID); }


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

    inline void bindToUnit(unsigned int unit=0)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mID);
    }

    inline GLuint getID() const { return mID;   };
    inline uvec3  size()  const { return mSize; };

    //=============================================================

    /**
     * @brief get_MAX_ARRAY_TEXTURE_LAYERS
     * @return the maximum number of layers the texture array supports
     */
    static GLuint get_MAX_ARRAY_TEXTURE_LAYERS()
    {
        GLint max_layers;
        glGetIntegerv (GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
        return max_layers;
    }

    /**
     * @brief get_MAX_TEXTURE_SIZE
     * @return the maximum dimension of the textures.
     */
    static GLuint get_MAX_TEXTURE_SIZE()
    {
        GLint max;
        glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
        return max;
    }


    private:
        GLuint  mID;
        uvec3   mSize;
        GLsizei mMipLevelCount;
};

};

#endif // GPUTEXTUREARRAY_H
