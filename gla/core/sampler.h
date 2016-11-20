/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EXP_SAMPLER_BASE
#define EXP_SAMPLER_BASE


#include "types.h"
#include "handle.h"
#include "image.h"

#include <vector>
#include <iostream>
#include <tuple>
#include <array>
#include <type_traits>


namespace gla {

struct GenSampler
{
    void operator()(GLuint & x)
    {
        //static int i=0;
        //x = ++i;
        //std::cout << "glGenBuffers(1, &m_ID)" << std::endl;
        glGenTextures(1, &x);
        GLA_LOGD << "Sampler Generated: " << x << std::endl;
    }
};

struct DestSampler
{
    void operator()(GLuint & x)
    {
        GLA_LOGD << "Sampler destroyed: " << x << std::endl;
        glDeleteTextures(1, &x);
    }
};



struct SamplerInfo
{
    //DataType   m_PixelDataType;

    glm::uvec3    m_Size; //with, height, depth (depth=6 for texture cubes, depth=1 for 2D and 2dArrays)
    std::uint32_t m_Layers = 1; // number of layers for array types

    GLenum     m_Type; // GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_CUBE
    GLenum     m_Format;
};

class Sampler :  public BaseHandle<GLuint, GenSampler, DestSampler,SamplerInfo>
{
public:

    enum Type : GLenum
    {
        TEXTURE_2D       = GL_TEXTURE_2D,
        TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
        TEXTURE_CUBE     = GL_TEXTURE_CUBE_MAP
    };

    enum Format : GLenum
    {
        R8               = GL_R8,
        RG8              = GL_RG8,
        RGB8             = GL_RGB8,
        RGBA8            = GL_RGBA8,
        DEPTH_COMPONENT  = GL_DEPTH_COMPONENT,
        DEPTH_COMPONENT16= GL_DEPTH_COMPONENT16,
        DEPTH_COMPONENT24= GL_DEPTH_COMPONENT24,
        DEPTH_COMPONENT32= GL_DEPTH_COMPONENT32,
        DEPTH_STENCIL    = GL_DEPTH_STENCIL,

       // R8               = GL_R8                   ,
        R8_SNORM	     = GL_R8_SNORM	            ,
        R16	             = GL_R16	                ,
        R16_SNORM	     = GL_R16_SNORM	        ,
      //  RG8		         = GL_RG8		            ,
        RG8_SNORM	     = GL_RG8_SNORM	 	    ,
        RG16	         = GL_RG16	           ,
        RG16_SNORM	     = GL_RG16_SNORM	        ,
        R3_G3_B2	     = GL_R3_G3_B2	            ,
        RGB4	         = GL_RGB4	                ,
        RGB5	         = GL_RGB5	                ,
        //RGB8	         = GL_RGB8	                ,
        RGB8_SNORM       = GL_RGB8_SNORM           ,
        RGB10	         = GL_RGB10	            ,
        RGB12	         = GL_RGB12	            ,
        RGB16_SNORM	     = GL_RGB16_SNORM	        ,
        RGBA2	         = GL_RGBA2	            ,
        RGBA4	         = GL_RGBA4	            ,
        RGB5_A1	         = GL_RGB5_A1	            ,
       // RGBA8	         = GL_RGBA8	            ,
        RGBA8_SNORM      = GL_RGBA8_SNORM          ,
        RGB10_A2	     = GL_RGB10_A2	            ,
        RGB10_A2UI	     = GL_RGB10_A2UI	        ,
        RGBA12           = GL_RGBA12               ,
        RGBA16           = GL_RGBA16               ,
        SRGB8            = GL_SRGB8                ,
        SRGB8_ALPHA8     = GL_SRGB8_ALPHA8         ,
        R16F             = GL_R16F                 ,
        RG16F            = GL_RG16F                ,
        RGB16F           = GL_RGB16F               ,
        RGBA16F          = GL_RGBA16F              ,
        R32F	         = GL_R32F	                ,
        RG32F	         = GL_RG32F	            ,
        RGB32F	         = GL_RGB32F	            ,
        RGBA32F	         = GL_RGBA32F	            ,
        R11F_G11F_B10F   = GL_R11F_G11F_B10F	    ,
        RGB9_E5	         = GL_RGB9_E5	            ,
        R8I	             = GL_R8I	                ,
        R8UI	         = GL_R8UI	                ,
        R16I	         = GL_R16I	                ,
        R16UI	         = GL_R16UI	            ,
        R32I	         = GL_R32I	                ,
        R32UI            = GL_R32UI                ,
        RG8I	         = GL_RG8I	           ,
        RG8UI	         = GL_RG8UI	            ,
        RG16I	         = GL_RG16I	            ,
        RG16UI	         = GL_RG16UI	            ,
        RG32I	         = GL_RG32I	            ,
        RG32UI	         = GL_RG32UI	            ,
        RGB8I	         = GL_RGB8I	            ,
        RGB8UI	         = GL_RGB8UI	            ,
        RGB16I	         = GL_RGB16I	            ,
        RGB16UI	         = GL_RGB16UI	            ,
        RGB32I	         = GL_RGB32I	            ,
        RGB32UI	         = GL_RGB32UI	            ,
        RGBA8I	         = GL_RGBA8I	            ,
        RGBA8UI	         = GL_RGBA8UI	            ,
        RGBA16I	         = GL_RGBA16I	            ,
        RGBA16UI	     = GL_RGBA16UI	            ,
        RGBA32I	         = GL_RGBA32I	            ,
        RGBA32UI	     = GL_RGBA32UI,

        COMPRESSED_RED                     = GL_COMPRESSED_RED,
        COMPRESSED_RG	                   = GL_COMPRESSED_RG	,
        COMPRESSED_RGB	                   = GL_COMPRESSED_RGB	,
        COMPRESSED_RGBA                    = GL_COMPRESSED_RGBA,
        COMPRESSED_SRGB                    = GL_COMPRESSED_SRGB,
        COMPRESSED_SRGB_ALPHA              = GL_COMPRESSED_SRGB_ALPHA,
        COMPRESSED_RED_RGTC1               = GL_COMPRESSED_RED_RGTC1,
        COMPRESSED_SIGNED_RED_RGTC1        = GL_COMPRESSED_SIGNED_RED_RGTC1,
        COMPRESSED_RG_RGTC2	               = GL_COMPRESSED_RG_RGTC2	,
        COMPRESSED_SIGNED_RG_RGTC2	       = GL_COMPRESSED_SIGNED_RG_RGTC2	,
        COMPRESSED_RGBA_BPTC_UNORM	       = GL_COMPRESSED_RGBA_BPTC_UNORM	,
        COMPRESSED_SRGB_ALPHA_BPTC_UNORM   = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM	,
        COMPRESSED_RGB_BPTC_SIGNED_FLOAT   = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT	,
        COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
    };

    enum Filter : GLenum {
        NEAREST                = GL_NEAREST,
        LINEAR                 = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class Wrap {
        CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,
        REPEAT                 = GL_REPEAT
    };

    Sampler()
    {
    }

    Sampler(const Image & I)
    {
        Format typ[] = {Sampler::R8, Sampler::RG8, Sampler::RGB8, Sampler::RGBA8};

        CreateTexture2D(I.size(), typ[I.getChannels()-1]);

        (*this) << I;

    }

    void Bind() const
    {
        glBindTexture(SharedData().m_Type, Get() );
    }

    void Unbind() const
    {
        glBindTexture(SharedData().m_Type, 0 );
    }

    Type GetType() const
    {
        return static_cast<Type>(SharedData().m_Type);
    }

    std::uint32_t GetLayers() const
    {
        return SharedData().m_Layers;
    }

    Format GetFormat() const
    {
        return static_cast<Format>(SharedData().m_Format);
    }

    uvec3 GetSize() const
    {
        return SharedData().m_Size;
    }


    void CreateTexture2D(const uvec2 & pSize, Format pFormat);
    void CreateTexture2DArray(const uvec2 & pSize, const std::uint32_t pLayers, Format pFormat);
    void CreateTextureCubeMap(const uvec2 & pSize, Format pFormat);

    void Copy( const ImageBase & T, std::uint32_t layer, const uvec2 & offset );


    class LayerReference
    {
    public:
        void operator << ( const ImageBase & T)
        {
            m_Samp.Copy( T, m_Layer, uvec2(0) );
        }

        private:
            LayerReference( Sampler & S, std::uint32_t l) : m_Samp(S), m_Layer(l)
            {

            }
            Sampler       & m_Samp;
            std::uint32_t   m_Layer;

            friend class Sampler;
    };

    LayerReference operator[](std::uint32_t layer)
    {
        return LayerReference(*this, layer);
    }


    void operator << ( const ImageBase & T)
    {
        Copy( T, 0, uvec2(0) );
    }

    /**
     *  Sets the Min and Mag filter for this texture
     *
     * @param Min The MIN filter to use
     * @param Mag The MAG filter to use
     */
    inline void SetFilter( Filter Min, Filter Mag)
    {
        Bind();

       // auto & I = m_Handle.__GetInfo();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLuint>(Min) );

        if( Mag == Filter::LINEAR_MIPMAP_LINEAR)  Mag = Filter::LINEAR;
        if( Mag == Filter::LINEAR_MIPMAP_NEAREST) Mag = Filter::NEAREST;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLuint>(Mag));

        //I.MagFilter = Mag;
        //I.MinFilter = Mag;

        Unbind();
    }

    inline void Repeat()
    {
        SetWrap(Wrap::REPEAT, Wrap::REPEAT);
    }

    inline void ClampToEdge()
    {
        SetWrap( Wrap::CLAMP_TO_EDGE, Wrap::CLAMP_TO_EDGE);
    }

    Sampler & ClampToEdgeX()
    {
        Bind();

        glTexParameteri( static_cast<GLenum>( GetType() ), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

        Unbind();
        return *this;
    }

    Sampler & ClampToEdgeY()
    {
        Bind();

        glTexParameteri( static_cast<GLenum>( GetType() ), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Unbind();
        return *this;
    }

    Sampler & ClampToEdgeZ()
    {
        Bind();

        glTexParameteri( static_cast<GLenum>( GetType() ), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Unbind();
        return *this;
    }

    Sampler & RepeatX()
    {
        Bind();

        glTexParameteri( static_cast<GLenum>( GetType() ), GL_TEXTURE_WRAP_S, GL_REPEAT);

        Unbind();
        return *this;
    }

    Sampler & RepeatY()
    {
        Bind();

        glTexParameteri( static_cast<GLenum>( GetType() ), GL_TEXTURE_WRAP_T, GL_REPEAT);

        Unbind();
        return *this;
    }

    Sampler & RepeatZ()
    {
        Bind();

        glTexParameteri( static_cast<GLenum>( GetType() ), GL_TEXTURE_WRAP_R, GL_REPEAT);

        Unbind();
        return *this;
    }
    inline void SetWrap( Wrap S_direction, Wrap T_direction)
    {
        Bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLuint)S_direction);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLuint)T_direction);

        //I.S_Wrap = S_direction;
        //I.T_Wrap = T_direction;

        Unbind();
    }

    void SetActive( unsigned int unit) const
    {
        glActiveTexture(GL_TEXTURE0+unit);
        Bind();
    }

    //=============================================================
    /**
     * @brief get_MAX_TEXTURE_SIZE
     * @return the maximum dimension of the textures.
     */
    static GLuint GetMaxTextureSize()
    {
        static GLint  max = 0;
        if(max!=0) return max;
        glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
        return max;
    }

    static GLuint GetMaxTextureImageUnits()
    {
        static GLint  max = 0;

        if( max != 0 ) return max;

        //glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
        return max;
    }
    //================================================================


    static Sampler DepthTexture( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::DEPTH_COMPONENT);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler DepthTexture16f( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::DEPTH_COMPONENT16);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler DepthTexture24f( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::DEPTH_COMPONENT24);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler DepthTexture32f( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::DEPTH_COMPONENT32);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }


    static Sampler RGBTexture( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::RGB8);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler RGBATexture( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::RGBA8);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler Vec3Texture16f( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::RGB16F);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler Vec4Texture16f( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::RGBA16F);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler Vec3Texture32f( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::RGB32F);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }

    static Sampler Vec4Texture32f( const glm::uvec2 & size)
    {
        Sampler D;
        D.CreateTexture2D( size, Format::RGBA32F);
        D.SetFilter( Filter::NEAREST , Filter::NEAREST );
        return D;
    }



};




inline void Sampler::CreateTexture2D(const uvec2 &pSize, Sampler::Format pFormat)
{
    Generate();

    SharedData().m_Type   = GL_TEXTURE_2D;
    SharedData().m_Size   = uvec3(pSize.x,pSize.y,1);
    SharedData().m_Layers = 1;

    std::cout << this->Get() << std::endl;
    CheckError();
    Bind();

    glTexStorage2D(GL_TEXTURE_2D,
                   1,
                   static_cast<GLenum>(pFormat),
                   static_cast<GLint>(pSize.x),
                   static_cast<GLint>(pSize.y));

    CheckError();
    Unbind();
}



inline void Sampler::CreateTexture2DArray(const uvec2 &pSize, const std::uint32_t pLayers, Sampler::Format pFormat)
{
    Generate();
    SharedData().m_Type   = GL_TEXTURE_2D_ARRAY;
    SharedData().m_Size   = uvec3(pSize.x,pSize.y,1);
    SharedData().m_Layers = pLayers;
    Bind();

    glTexStorage3D(GL_TEXTURE_2D_ARRAY,
                   1,
                   static_cast<GLenum>(pFormat),
                   static_cast<GLint>(pSize.x),
                   static_cast<GLint>(pSize.y),
                   static_cast<GLsizei>(pLayers));

    Unbind();
}


inline void Sampler::CreateTextureCubeMap(const uvec2 &pSize, Sampler::Format pFormat)
{
    Generate();
    SharedData().m_Type   = GL_TEXTURE_CUBE_MAP;
    SharedData().m_Size   = uvec3(pSize.x,pSize.y,1);
    SharedData().m_Layers = 6;
    Bind();

  glTexStorage2D(GL_TEXTURE_CUBE_MAP,
                 1,
                 static_cast<GLenum>(pFormat),
                 static_cast<GLint>(pSize.x),
                 static_cast<GLint>(pSize.y));

// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_DEPTH_COMPONENT,  static_cast<GLint>(pSize.x), static_cast<GLint>(pSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_DEPTH_COMPONENT,  static_cast<GLint>(pSize.x), static_cast<GLint>(pSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_DEPTH_COMPONENT,  static_cast<GLint>(pSize.x), static_cast<GLint>(pSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_DEPTH_COMPONENT,  static_cast<GLint>(pSize.x), static_cast<GLint>(pSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_DEPTH_COMPONENT,  static_cast<GLint>(pSize.x), static_cast<GLint>(pSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_DEPTH_COMPONENT,  static_cast<GLint>(pSize.x), static_cast<GLint>(pSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    Unbind();
}

inline void Sampler::Copy( const ImageBase & T, std::uint32_t pLayer, const uvec2 & pOffset )
{
    Bind();

    if( Get() == 0)
    {
        throw std::runtime_error("Error: Sampler2DArray not generated\n");
    }

    auto mSize       = SharedData().m_Size;
    //auto mComponents = SharedData().m_Components;
    auto m_Depth     = SharedData().m_Layers;

    if( pLayer >= m_Depth)
    {
        throw std::runtime_error("ERROR: Layer number is larger than the allocated size of the TextureArray.");
    }

    if( T.size().x + pOffset.x > mSize.x || T.size().y + pOffset.y > mSize.y )
    {
        throw std::runtime_error("ERROR: The Image is too large for the Sampler2DArray");
    }

    //if( T.getChannels() != mComponents )
    //{
    //    char msg[256];
    //    sprintf(msg, "ERROR: Texture and TextureArray have missmatched components, Texture = %d   Texture Array = %d", (int)mComponents, (int)T.getChannels() );
    //    throw std::runtime_error(std::string(msg) );
    //}

    GLenum format[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};


    if( SharedData().m_Type == GL_TEXTURE_2D_ARRAY)
    {
        glTexSubImage3D(  GL_TEXTURE_2D_ARRAY,
                          0,                      // level
                          pOffset.x,              // x-offset
                          pOffset.y,              // y-offset
                          pLayer,                  // z-offset
                          T.size().x,             // width
                          T.size().y,             // height
                          1,                      // depth
                          format[T.getChannels()-1],  // format
                          GL_UNSIGNED_BYTE,       // type
                          T.getRawData()          // image data
                       );
            CheckError();
    }
    else if( SharedData().m_Type == GL_TEXTURE_2D)
    {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        pOffset.x, pOffset.y,
                        T.size().x,
                        T.size().y,
                        format[T.getChannels()-1],  // format
                        GL_UNSIGNED_BYTE,
                        T.getRawData() );
        CheckError();
    }
    else if( SharedData().m_Type == GL_TEXTURE_CUBE_MAP)
    {
        glTexSubImage2D(  GL_TEXTURE_CUBE_MAP_POSITIVE_X+static_cast<GLenum>(pLayer),
                          0,
                          pOffset.x,              // x-offset
                          pOffset.y,              // y-offset
                          T.size().x,             // width
                          T.size().y,             // height
                          format[T.getChannels()-1],  // format
                          GL_UNSIGNED_BYTE,       // type
                          T.getRawData()          // image data
                       );
    }
    Unbind();
}
/*

inline void Sampler::PasteSubImage( const uvec2 & xy, const ImageBase & T, int level)
{
    if( !Get() ) return;

    auto d = T.getRawData();

    if(!d) return;

    //have a bigger image bigImage and the
    // subImage sub
    //std::cout  << "Copying subimage over\n";
    //std::cout  << "        x: " << xy.x << std::endl;
    //std::cout  << "        y: " << xy.y << std::endl;
    //std::cout <<  "        w: " << T.size().x << std::endl;
    //std::cout <<  "        h: " << T.size().y << std::endl;
    Bind();


    //const
    //SamplerFormat internal[] = {SamplerFormat::RED ,
    //                            SamplerFormat::RG  ,
    //                            SamplerFormat::RGB ,
    //                            SamplerFormat::RGBA};

    GLenum imformat[] = {  GL_RED ,
                           GL_RG,
                           GL_RGB,
                           GL_RGBA};

    //GLenum Format[] = {GL_RED, 0, GL_RGB, GL_RGBA};
    // Note, i/j might have the origin int he bottom left corner instead of top left.
 //   GLenum Format[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
   // GLA_DOUT  << "PASTING SUB IMAGE: " << T.getChannels()-1 <<std::endl;
    glTexSubImage2D(GL_TEXTURE_2D,
                    level,
                    xy.x, xy.y,
                    T.size().x,
                    T.size().y,
                    imformat[T.getChannels()-1],
                    (GLenum)GL_UNSIGNED_BYTE,
                    d );

    glGenerateMipmap(GL_TEXTURE_2D);




}
*/
}

#endif
