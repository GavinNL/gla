#include "gla/texture.h"
#include <string.h>
#include <iostream>
#include <gla/stb/stb_image.h>


namespace gla {

#define CLAMP(a,A,B) std::min( B, std::max(a,A) )

//=================================================================================================================
// GPUTexture
//=================================================================================================================
//Texture GPUTexture::toCPU()
//{
//    bind();
//
//    Texture T(mDim.x, mDim.y);
//
//    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA,  GL_UNSIGNED_BYTE, (void*)T.getRawData() );
//
//    return std::move(T);
//}
//=================================================================================================================



//=================================================================================================================
// Texture
//=================================================================================================================
// template<class PixelType>
// Texture<PixelType> LoadTexture(const std::string &path)
// {
//     Texture<PixelType> T;
//     T.loadFromPath(path);
//     return std::move(T);
//     //return T;
// }
//
//
//







 /*======================================================================================
 Definitions for ChannelReferences
 =======================================================================================*/



}


#define GLA_IMAGE_IMPLEMENTATION
#include <gla/stb/stb_image.h>
