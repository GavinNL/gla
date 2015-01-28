#include "glre/texture.h"
#include <string.h>

glre::Texture::Texture() :mTextureID(0), mWidth(0), mHeight(0), mData(0)
{

}


glre::Texture::~Texture()
{
    clearCPU();
    clearGPU();
}

void glre::Texture::sendToGPU()
{
    if(!mData) return;

    // if w and h are both zero, then copy the entire image to the GPU

        clearGPU();
        //==========================================================
        // Copy the Texture to the GPU
        //==========================================================
        glGenTextures(1, &mTextureID);
        glBindTexture(GL_TEXTURE_2D, mTextureID);

        glTexImage2D( GL_TEXTURE_2D, 0 , GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,  (void*)mData );

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //==========================================================

}

void glre::Texture::clearGPU()
{

    if( mTextureID)
    {
        glDeleteTextures(1, &mTextureID);
        mTextureID=0;
    }

}

void glre::Texture::clearCPU()
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
        std::cout << "fromMemory error: " << x << "," << y << "\n";
    }

   // throw Exception("Texture::fromMemory() - Unknown Image format.");
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
    clearCPU();
    clearGPU();

    mWidth = width;
    mHeight = height;

    mData = new glre::ucol4[mWidth*mHeight];
    memcpy( mData, (void*)buffer, mWidth*mHeight*4);

    for(int i=0;i<mWidth*mHeight;i++) std::swap(mData[i].r, mData[i].b);

}
#define GLRE_IMAGE_IMPLEMENTATION
#include <glre/stb/stb_image.h>
