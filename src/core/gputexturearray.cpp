#include "gla/core/gputexturearray.h"
#include "gla/core/texture.h"

namespace gla {



GPUTextureArray::GPUTextureArray(uint width, uint height, uint depth) : mID(0), mSize(0,0,0),  mMipLevelCount(0)
{
    create( uvec2(width, height), depth, 1);
}


void GPUTextureArray::create( uvec2 size, unsigned int depth, int MipMapCount )
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

    if( !mID )
    {
        throw gla::GLA_EXCEPTION("ERROR: error generating textures for TextureArray2D");
    }

    bind();

    glTexStorage3D( GL_TEXTURE_2D_ARRAY, MipMapCount, GL_RGBA8, size.x, size.y, depth);

    GetGLError();

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S    , GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T    , GL_CLAMP_TO_EDGE);

    GetGLError();

    mSize = uvec3(size.x, size.y, depth);
}

//void GPUTextureArray::SetLayer(const Texture & T, uint Layer, const uvec2 & pOffset)
//{
//    bind();

//    if( Layer >= mSize.z)
//    {
//        throw gla::GLA_EXCEPTION("ERROR: Layer number is larger than the allocated size of the TextureArray.");
//    }

//    if( T.size().x + pOffset.x > mSize.x || T.size().y + pOffset.y > mSize.y )
//    {
//        throw gla::GLA_EXCEPTION("ERROR: The texture dimensions do not match");
//    }

//    glTexSubImage3D(  GL_TEXTURE_2D_ARRAY,
//                      0,                // level
//                      pOffset.x,        // x-offset
//                      pOffset.y,        // y-offset
//                      Layer,                // z-offset
//                      T.size().x,       // width
//                      T.size().y,       // height
//                      1,            // depth
//                      GL_RGBA,          // format
//                      GL_UNSIGNED_BYTE, // type
//                      T.getRawData()    // image data
//                   );
//}

GPUTextureArray::GPUTextureArray(uvec3 size)
{

}


GPUTextureArray::GPUTextureArray() : mID(0), mSize(0,0,0), mMipLevelCount(1)
{

}

GPUTextureArray::~GPUTextureArray()
{

}

}
