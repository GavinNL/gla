#include "glre/texture.h"
#include <string.h>
#include <iostream>
#include <glre/stb/stb_image.h>


namespace glre {

#define CLAMP(a,A,B) std::min( B, std::max(a,A) )

//=================================================================================================================
// GPUTexture
//=================================================================================================================
Texture GPUTexture::toCPU()
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

GPUTexture Texture::toGPU()
{

    GPUTexture GPU;

    glGenTextures(1, &GPU.mTextureID);

    if( !GPU.mTextureID )
    {
        throw glre::GLRE_EXCEPTION("Error generating Texture on the GPU");
    }

    GPU.mDim = mDim;

    glBindTexture(GL_TEXTURE_2D, GPU.mTextureID);

    glTexImage2D( GL_TEXTURE_2D, 0 , GL_RGBA, mDim.x, mDim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE,  (void*)mData );

    GPU.setFilter( GPUTexture::LINEAR, GPUTexture::LINEAR );

    return GPU;
}


void Texture::clear()
{
    if( mData ) delete [] mData;
    mData = 0;
}


void Texture::loadFromMemory( unsigned char * Buffer, int buffersize)
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


void Texture::loadFromPath( const std::string & path)
{

    int x, y, comp;

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


void Texture::_handleRawPixels(unsigned char * buffer, uint width, uint height)
{
    clear();

    mDim = {width, height};

    mData = new glre::ucol4[width*height];
    memcpy( mData, (void*)buffer, width*height*4);

    //for(int i=0;i<width*height;i++) std::swap(mData[i].r, mData[i].b);

}


Texture LoadTexture(const std::string &path)
{
    Texture T;
    T.loadFromPath(path);
    return std::move(T);
    //return T;
}



 Texture        Texture::operator+(  Texture & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0;x<siz.x;x++)
        for(int y=0; y<siz.y; y++)
        {
            T(x,y) = (*this)(x,y) + c(x,y);
        }

    return( std::move(T) );

};

 Texture        Texture::operator-(  Texture & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            T(x,y) = (*this)(x,y) - c(x,y);
        }

    return( std::move(T) );
};

 void           Texture::operator+=( Texture & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            (*this)(x,y) += c(x,y);
        }

};

 void           Texture::operator-=( Texture & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            (*this)(x,y) -= c(x,y);
        }
};


 Texture        Texture::operator+(  ChannelReference & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0;x<siz.x;x++)
        for(int y=0; y<siz.y; y++)
        {
            T(x,y) = (*this)(x,y) + c(x,y);
        }

    return( std::move(T) );

};

 Texture        Texture::operator-(  ChannelReference & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            T(x,y) = (*this)(x,y) - c(x,y);
        }

    return( std::move(T) );
};

 void           Texture::operator+=( ChannelReference & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            (*this)(x,y) += c(x,y);
        }

};

 void           Texture::operator-=( ChannelReference & c)
{
    uvec2 siz = glm::min( size(), c.size() );
    Texture T(siz.x,siz.y);

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            (*this)(x,y) -= c(x,y);
        }
};


 Texture        Texture::operator+(  unsigned char c)
{
    uvec2 siz = size();
    Texture T(siz.x,siz.y);

    for(int x=0;x<siz.x;x++)
        for(int y=0; y<siz.y; y++)
        {
            T(x,y) = (*this)(x,y) + ucol4(c);
        }

    return( std::move(T) );

};

 Texture        Texture::operator-(  unsigned char c)
{
    uvec2 siz = size();
    Texture T(siz.x,siz.y);

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            T(x,y) = (*this)(x,y) - ucol4(c);
        }

    return( std::move(T) );
};

 void           Texture::operator+=( unsigned char c)
{
    uvec2 siz = size();

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            (*this)(x,y) += ucol4( c );
        }

};

 void           Texture::operator-=( unsigned char c)
{
    uvec2 siz = size();

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            (*this)(x,y) -= ucol4(c);
        }
};

 Texture& Texture::operator=(        unsigned char c)
{
    uvec2 siz = size();

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            (*this)(x,y) = ucol4(c);
        }

    return *this;
}

 Texture& Texture::operator=(        ChannelReference & c)
{
    uvec2 siz = size();

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            auto col = c(x,y);
            (*this)(x,y) = ucol4(col);
        }

    return *this;
}

 Texture& Texture::operator=(  const TextureChannel   & c)
{
    uvec2 siz = size();

    for(int x=0; x < siz.x; x++)
        for(int y=0; y < siz.y; y++)
        {
            auto col = c.get(x,y);
            (*this)(x,y) = ucol4( col);
        }

    return *this;

}


Texture& Texture::operator=( std::function<vec4(const vec2 &)> F)
{
     uvec2 s = size();

     for(uint x=0; x < s.x; x += 1)
     {
         for(uint y=0; y < s.y; y += 1)
         {
             (*this)( (int)x, (int)y ) = ucol4( F( vec2(x,y) / vec2(s) )*255.0f);
         }
     }
};

Texture& Texture::operator=( std::function<float(const vec2 &)> F)
{
     uvec2 s = size();

     for(uint x=0; x < s.x; x += 1)
     {
         for(uint y=0; y < s.y; y += 1)
         {
             (*this)( (int)x, (int)y ) = ucol4( F( vec2(x,y) / vec2(s) )*255.0f );
         }
     }
};




 /*======================================================================================
 Definitions for ChannelReferences
 =======================================================================================*/

 ChannelReference& ChannelReference::operator=(ChannelReference & c)
 {

     uvec2 s = mTexture->size();
     for(int x=0; x < s.x; x++)
         for(int y=0; y < s.y; y++)
         {
             (*mTexture)(x,y)[mIndex] = c( (int)x, (int)y);
         }

     return *this;
 }

 TextureChannel ChannelReference::operator+( ChannelReference & c)
 {
     uvec2 s = mTexture->size();

     TextureChannel C( s.x, s.y );

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
             int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) + static_cast<int>( c(x,y) );
             C(x,y) = CLAMP(l, 0,255);
     }

     return( std::move(C) );
 }

 TextureChannel ChannelReference::operator-( ChannelReference & c)
 {
     uvec2 s = mTexture->size();

     TextureChannel C( s.x, s.y );

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
             int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) - static_cast<int>( c(x,y) );
             C(x,y) = CLAMP(l, 0,255);
     }

     return( std::move(C) );
 }


 void ChannelReference::operator+=( ChannelReference & c)
 {
     uvec2 s = mTexture->size();

     TextureChannel C( s.x, s.y );

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
             int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) + static_cast<int>( c(x,y) );
             C(x,y) = CLAMP(l, 0,255);
     }

 }

 void ChannelReference::operator-=( ChannelReference & c)
 {
     uvec2 s = mTexture->size();

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
         int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) - static_cast<int>( c(x,y) );
         (*mTexture)(x,y)[mIndex] = CLAMP(l, 0,255);
     }

 }

 void ChannelReference::operator+=( unsigned char  c )
 {
     uvec2 s = mTexture->size();

     TextureChannel C( s.x, s.y );

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
         int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) + static_cast<int>( c);
         C(x,y) = CLAMP(l, 0,255);
     }

 }

 void ChannelReference::operator-=( unsigned char  c )
 {
     uvec2 s = mTexture->size();

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
         int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) - static_cast<int>( c );
         (*mTexture)(x,y)[mIndex] = CLAMP(l, 0,255);
     }

 }


 TextureChannel ChannelReference::operator+( unsigned char  c )
 {
     uvec2 s = mTexture->size();


     TextureChannel C( s.x, s.y );

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
         int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) + static_cast<int>( c );
         C(x,y) = CLAMP(l, 0,255);
     }

     return( std::move(C) );
 }

 TextureChannel ChannelReference::operator-( unsigned char  c)
 {
     uvec2 s = mTexture->size();


     TextureChannel C( s.x, s.y );

     for(int x=0; x < s.x; x++)  for(int y=0; y < s.y; y++)
     {
         int l = static_cast<int>( (*mTexture)(x,y)[mIndex] ) - static_cast<int>( c );
         C(x,y) = CLAMP(l, 0,255);
     }

     return( std::move(C) );
 }


ChannelReference& ChannelReference::operator=(const TextureChannel & c)
{
    uvec2 s = mTexture->size();

    for(int x=0; x < s.x; x++)
        for(int y=0; y < s.y; y++)
            (*mTexture)(x,y)[mIndex] = c.get( x, y);

    return *this;
}

ChannelReference& ChannelReference::operator=(unsigned char c)
{

    uvec2 s = mTexture->size();

    for(int x=0; x < s.x; x++)
        for(int y=0; y < s.y; y++)
            (*mTexture)(x,y)[mIndex] = c;

    return *this;
}



ChannelReference&       ChannelReference::operator=( std::function<        float(const  vec2 &)> F)
{

     uvec2 s = size();

     for(uint x=0; x < s.x; x += 1)
     {
         for(uint y=0; y < s.y; y += 1)
         {
             (*mTexture)( (int)x, (int)y )[mIndex] = static_cast<unsigned char>( F( vec2(x,y) / vec2(s) )*255.0f );
         }
     }

     return *this;
};


/*==============================================================================
Texture Channel Definitions
==============================================================================*/


TextureChannel&       TextureChannel::operator=( std::function<        float(const  vec2 &)> F)
{

     uvec2 s = size();

     for(uint x=0; x < s.x; x += 1)
     {
         for(uint y=0; y < s.y; y += 1)
         {
             (*this)( (int)x, (int)y ) = static_cast<unsigned char>( F( vec2(x,y) / vec2(s) )*255.0f );
         }
     }

     return *this;
};


TextureChannel TextureChannel::operator+(TextureChannel & R)
{
    TextureChannel D( mDim.x, mDim.y );
    for(int i=0;i<mDim.x*mDim.y;i++)
        D.mData[i] = R.mData[i] + mData[i];

    return( std::move(D) );
}

TextureChannel TextureChannel::operator-(TextureChannel & R)
{
    TextureChannel D( mDim.x, mDim.y );
    for(int i=0;i<mDim.x*mDim.y;i++)
        D.mData[i] = R.mData[i] - mData[i];

    return( std::move(D) );
}

TextureChannel TextureChannel::operator-(unsigned char r)
{
    TextureChannel D( mDim.x, mDim.y );
    for(int i=0;i<mDim.x*mDim.y;i++)
        D.mData[i] = mData[i] - r;

    return( std::move(D) );
}

TextureChannel TextureChannel::operator+(unsigned char r)
{
    TextureChannel D( mDim.x, mDim.y );
    for(int i=0;i<mDim.x*mDim.y;i++)
        D.mData[i] = mData[i] + r;

    return( std::move(D) );
}

//
void TextureChannel::operator+=(TextureChannel & R)
{
    uvec2 s = glm::min( mDim, R.size() );

    for(int x=0; x < s.x; x++)
        for(int y=0; y < s.y; y++)
        {
            (*(this))(x,y) += R(x,y);
        }

}

void TextureChannel::operator-=(TextureChannel & R)
{

    uvec2 s = glm::min( mDim, R.size() );

    for(int x=0; x < s.x; x++)
        for(int y=0; y < s.y; y++)
        {
            (*(this))(x,y) -= R(x,y);
        }

}

void TextureChannel::operator-=(unsigned char r)
{

    uvec2 s = mDim;

    for(int x=0; x < s.x; x++)
        for(int y=0; y < s.y; y++)
        {
            (*(this))(x,y) -= r;
        }

}

void TextureChannel::operator+=(unsigned char r)
{

    uvec2 s = mDim;

    for(int x=0; x < s.x; x++)
        for(int y=0; y < s.y; y++)
        {
            (*(this))(x,y) += r;
        }
}

}


#define GLRE_IMAGE_IMPLEMENTATION
#include <glre/stb/stb_image.h>
