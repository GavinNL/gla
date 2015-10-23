#ifndef GLA_GPUTEXTUREARRAY_H
#define GLA_GPUTEXTUREARRAY_H

#include <gla/types.h>
#include <memory>

namespace gla {


struct GPUTextureArrayInfo
{
    uvec3        size;
    int          mipmaps;
    unsigned int channels;
};

//===========================================================================
// GPUTextureArray
//   - an array of 2D textures stored on the GPU.
//===========================================================================
class GPUTextureArray
{

public:
    GPUTextureArray(){};

    //GPUTextureArray(uint width, uint height, uint depth, uint comp) : mID(0), mSize(0,0,0), mMipLevelCount(1) , mComponents(comp) {};

    GPUTextureArray(uvec3 size){}
    ~GPUTextureArray(){};

    /**
     * @brief create Creates the texture array on the GPU
     * @param size the width and height dimensions of the textures
     * @param depth the number of textures to allocate space for
     * @param MipMapCount the mipmap level, default is 1
     */
    void create(uvec2 size, unsigned int depth, int MipMapCount=1, unsigned int components=4);



    /**
     * @brief SetLayer - Copies a CPU texture into a particular depth in the Texture Array
     * @param T - CPU texture to copy. This must be the same dimensions as the TextureArray
     * @param Layer - the layer number to copy the texture into
     * @param pOffset - an offset parameter.
     */
    void SetLayer(const Texture & T, uint Layer, const uvec2 & pOffset=uvec2(0,0))
    {
        bind();

        auto mSize       = mInfo->size;
        auto mComponents = mInfo->channels;

        if( Layer >= mSize.z)
        {
            throw gla::GLA_EXCEPTION("ERROR: Layer number is larger than the allocated size of the TextureArray.");
        }

        if( T.size().x + pOffset.x > mSize.x || T.size().y + pOffset.y > mSize.y )
        {
            throw gla::GLA_EXCEPTION("ERROR: The texture dimensions do not match");
        }

        if( T.getChannels() != mComponents )
        {
            char msg[256];
            sprintf(msg, "ERROR: Texture and TextureArray have missmatched components, Texture = %d   Texture Array = %d", mComponents, T.getChannels() );
            throw gla::GLA_EXCEPTION(msg);
        }

        GLuint format[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};

        glTexSubImage3D(  GL_TEXTURE_2D_ARRAY,
                          0,                      // level
                          pOffset.x,              // x-offset
                          pOffset.y,              // y-offset
                          Layer,                  // z-offset
                          T.size().x,             // width
                          T.size().y,             // height
                          1,                      // depth
                          format[T.getChannels()-1],  // format
                          GL_UNSIGNED_BYTE,       // type
                          T.getRawData()          // image data
                       );
    }

    inline void bind() { glBindTexture(GL_TEXTURE_2D_ARRAY, mID); }


    void clear()
    {
        mInfo = 0;
        mID   = 0;
        //if(mID)   glDeleteTextures(1, &mID);
        //
        //mSize          = {0,0,0};
        //mMipLevelCount = 0;
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
    //inline uvec3  size()  const { mInfo!=0 ? mInfo->size : 0; };

    inline const GPUTextureArrayInfo Info() { return mInfo!=0 ? *mInfo.get() : GPUTextureArrayInfo(); }
    //=============================================================

    /**
     * @brief get_MAX_ARRAY_TEXTURE_LAYERS
     * @return the maximum number of layers the texture array supports
     */
    static GLuint get_MAX_ARRAY_TEXTURE_LAYERS()
    {
        static GLint max_layers=0;
        if(max_layers) return max_layers;

        glGetIntegerv (GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
        return max_layers;
    }

    /**
     * @brief get_MAX_TEXTURE_SIZE
     * @return the maximum dimension of the textures.
     */
    static GLuint get_MAX_TEXTURE_SIZE()
    {
        GLint max=0;

        if(max) return max;

        glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
        return max;
    }


    private:
        GLuint  mID;
//        uvec3   mSize;
//        unsigned int mComponents;
//        GLsizei mMipLevelCount;

        std::shared_ptr<GPUTextureArrayInfo> mInfo;
};



//============================================
inline void GPUTextureArray::create(uvec2 size, unsigned int depth, int MipMapCount, unsigned int components)
{
    try
    {
        GetGLError();
    } catch (std::exception & e)
    {

    }

    if( mID ) clear();

    glGenTextures(1, &mID);

    GetGLError();
    printf("Generated textures\n");
    if( !mID )
    {
        throw gla::GLA_EXCEPTION("ERROR: error generating textures for TextureArray2D");
    }

    bind();

    GLuint format[4] = {GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};
    //glTexStorage3D( GL_TEXTURE_2D_ARRAY, MipMapCount, format[components], size.x, size.y, depth);
    glTexStorage3D( GL_TEXTURE_2D_ARRAY, MipMapCount, format[components-1], size.x, size.y, depth);

    GetGLError();
    printf("glTexStorage complete\n");
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S    , GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T    , GL_REPEAT);

    GetGLError();

    //mSize       = uvec3(size.x, size.y, depth);
    //mComponents = components;

    auto id = mID;
    mInfo = std::shared_ptr<GPUTextureArrayInfo>( new GPUTextureArrayInfo, [=](GPUTextureArrayInfo* a){ delete a; glDeleteTextures(1, &id); std::cout << "Deleting Texture Array: " << id << std::endl; } );
    mInfo->channels = components;
    mInfo->size     = uvec3(size.x,size.y,depth);
    mInfo->mipmaps  = MipMapCount;

};




};

#endif // GPUTEXTUREARRAY_H
