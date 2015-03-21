#include "glre/texture.h"
#include <string.h>
#include <iostream>
#include <glre/stb/stb_image.h>


//=================================================================================================================
// GPUTexture
//=================================================================================================================
glre::Texture glre::GPUTexture::toCPU()
{
    bind();

    Texture T(mDim.x, mDim.y);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA,  GL_UNSIGNED_BYTE, (void*)T.getRawData() );

    return std::move(T);
}

//=================================================================================================================



//=================================================================================================================
// Texture
//=================================================================================================================
glre::Texture::Texture() : mData(0)
{

}

glre::Texture::Texture(const std::string & path) :  mData(0)
{
    loadFromPath(path);
}


glre::Texture::Texture(uint width, uint height) : mData(0)
{
    mDim.x = width;
    mDim.y = height;
    mData = new glre::ucol4[ mDim[0]*mDim[1] ];
}

glre::Texture::~Texture()
{
    clear();
}

glre::GPUTexture glre::Texture::toGPU()
{

    GPUTexture GPU;

    glGenTextures(1, &GPU.mTextureID);

    if( !GPU.mTextureID )
    {
        throw glre::GLRE_EXCEPTION("Error generating Texture on the GPU");
    }
    //printf(" size: %d %d.    ID: %d\n", mDim.x, mDim.y, GPU.mTextureID);

    GPU.mDim = mDim;

    glBindTexture(GL_TEXTURE_2D, GPU.mTextureID);

    glTexImage2D( GL_TEXTURE_2D, 0 , GL_RGBA, mDim.x, mDim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE,  (void*)mData );

    GPU.setFilter(GPUTexture::NEAREST, GPUTexture::NEAREST);

    return GPU;
}


void glre::Texture::clear()
{
    if( mData ) delete [] mData;
    mData = 0;
}


void glre::Texture::loadFromMemory( unsigned char * Buffer, int buffersize)
{
    int x, y, comp;
    glre_uc           * img = glre_load_from_memory( (glre_uc*) Buffer, buffersize, &x, &y, &comp, 4);

    if( img )
    {
         _handleRawPixels(img, static_cast<unsigned int>( x ), static_cast<unsigned int>( y ) );

         free(img);
    } else {
        std::cout << "from Memory error: " << x << "," << y << "\n";
    }

}


void glre::Texture::loadFromPath( const std::string & path)
{

    int x,y,comp;
    //==========================================================
    // Load the Texture from an image file.
    //==========================================================
    unsigned char * img = glre_load(path.c_str(), &x, &y, &comp, 4);

    if( img )
    {
         _handleRawPixels(img, static_cast<unsigned int>( x ), static_cast<unsigned int>( y ) );

         free(img);
    } else {
        std::cout << "Error loading texture: " << path << std::endl;
    }

}




void glre::Texture::_handleRawPixels(unsigned char * buffer, uint width, uint height)
{
    clear();

    mDim = {width, height};

    mData = new glre::ucol4[width*height];
    memcpy( mData, (void*)buffer, width*height*4);

    //for(int i=0;i<width*height;i++) std::swap(mData[i].r, mData[i].b);

}

glre::Texture::Texture(Texture & T)
{

    mDim  = T.mDim;

    mData = new glre::ucol4[mDim[0]*mDim[1]];
    memcpy( mData, (void*)T.mData, mDim[0]*mDim[1]);
    std::cout << "Texture copy constructor\n";
}



glre::Texture glre::LoadTexture(const std::string &path)
{
    Texture T;
    T.loadFromPath(path);
    return std::move(T);
    //return T;
}


#define GLRE_IMAGE_IMPLEMENTATION
#include <glre/stb/stb_image.h>
