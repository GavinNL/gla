#ifndef EXP_Sample2d
#define EXP_Sample2d


#include "types.h"
#include "handle.h"
#include "image.h"
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <tuple>
#include <array>
#include <type_traits>


namespace gla { namespace experimental
{

struct GenSampler2D
{
    void operator()(GLuint & x)
    {
        //static int i=0;
        //x = ++i;
        //std::cout << "glGenBuffers(1, &m_ID)" << std::endl;
        glGenTextures(1, &x);
        std::cout << "Sampler2D Generated: " << x << std::endl;
    }
};

struct DestSampler2D
{
    void operator()(GLuint & x)
    {
        std::cout << "Sampler2D destroyed: " << x << std::endl;
        glDeleteTextures(1, &x);
    }
};


enum class SamplerFormat
{
    RED            = GL_RED,
    RG             = GL_RG,
    RGB            = GL_RGB,
    BGR            = GL_BGR,
    RGBA           = GL_RGBA,
    BGRA           = GL_BGRA,
    DEPTH_COMPONENT= GL_DEPTH_COMPONENT,
    DEPTH_STENCIL  = GL_DEPTH_STENCIL,

    R8               = GL_R8                   ,
    R8_SNORM	     = GL_R8_SNORM	            ,
    R16	             = GL_R16	                ,
    R16_SNORM	     = GL_R16_SNORM	        ,
    RG8		         = GL_RG8		            ,
    RG8_SNORM	     = GL_RG8_SNORM	 	    ,
    RG16	         = GL_RG16	           ,
    RG16_SNORM	     = GL_RG16_SNORM	        ,
    R3_G3_B2	     = GL_R3_G3_B2	            ,
    RGB4	         = GL_RGB4	                ,
    RGB5	         = GL_RGB5	                ,
    RGB8	         = GL_RGB8	                ,
    RGB8_SNORM       = GL_RGB8_SNORM           ,
    RGB10	         = GL_RGB10	            ,
    RGB12	         = GL_RGB12	            ,
    RGB16_SNORM	     = GL_RGB16_SNORM	        ,
    RGBA2	         = GL_RGBA2	            ,
    RGBA4	         = GL_RGBA4	            ,
    RGB5_A1	         = GL_RGB5_A1	            ,
    RGBA8	         = GL_RGBA8	            ,
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


enum class ImageFormat
{
    RED               = GL_RED,
    RG                = GL_RG,
    RGB               = GL_RGB,
    BGR               = GL_BGR,
    RGBA              = GL_RGBA,
    BGRA              = GL_BGRA,
    RED_INTEGER       = GL_RED_INTEGER,
    RG_INTEGER        = GL_RG_INTEGER,
    RGB_INTEGER       = GL_RGB_INTEGER,
    BGR_INTEGER       = GL_BGR_INTEGER,
    RGBA_INTEGER      = GL_RGBA_INTEGER,
    BGRA_INTEGER      = GL_BGRA_INTEGER,
    STENCIL_INDEX     = GL_STENCIL_INDEX,
    DEPTH_COMPONENT   = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL     = GL_DEPTH_STENCIL
};


enum class SamplerWrap {
    CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,
    REPEAT                 = GL_REPEAT
};


enum class SamplerFilter {
    NEAREST                = GL_NEAREST,
    LINEAR                 = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
};


class Sampler2D :  public BaseHandle<GLuint, GenSampler2D, DestSampler2D>
{
public:


    Sampler2D()
    {

    }

    Sampler2D( const glm::uvec2 & size,
                bool              MipMaps        =  false,
                SamplerFormat     InternalFormat =  SamplerFormat::RGBA,
                ImageFormat       Format         =  ImageFormat::RGBA,
                DataType          Type           =  DataType::UNSIGNED_BYTE
                )
    {
        Create(size, MipMaps, InternalFormat, Format, Type);
    }

    Sampler2D( const Image & img, bool mipmaps =true )
    {
        int numChannels = img.getChannels();

        const
        SamplerFormat internal[] = {SamplerFormat::RED ,
                                    SamplerFormat::RG  ,
                                    SamplerFormat::RGB ,
                                    SamplerFormat::RGBA};

        const
        ImageFormat imformat[] = {  ImageFormat::RED ,
                                    ImageFormat::RG  ,
                                    ImageFormat::RGB ,
                                    ImageFormat::RGBA};

        std::cout << "Creating Sampler: " << img.size().x << ", " << img.size().y << std::endl;
        //std::cout << "                : " << img.size().x << ", " << img.size().y << std::endl;

        Create( img.size(),
                internal[numChannels-1],
                imformat[numChannels-1],
                DataType::UNSIGNED_BYTE,
                img.getRawData() ,
                mipmaps);

    }

    void Bind()
    {
        glBindTexture(GL_TEXTURE_2D, Get() );
    }

    void Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0 );
    }


    inline bool Create(const uvec2 & size,
                       bool MipMaps=true,

                       SamplerFormat InternalFormat = SamplerFormat::RGBA,
                       ImageFormat   texformat      = ImageFormat::RGBA,
                       DataType            datatype = DataType::UNSIGNED_BYTE
                       )
    {

        return( Create(size, InternalFormat, texformat, datatype, 0, MipMaps) );
    }

    /**
     * @brief create Creates a blank texture
     * @param size The size of the texture to create on the GPU.
     */
    inline bool Create(const uvec2                         &size,
                       SamplerFormat     InternalFormat =  SamplerFormat::RGBA,
                       ImageFormat       Format         =  ImageFormat::RGBA,
                       DataType          Type           =  DataType::UNSIGNED_BYTE,
                       void*             Data           =  0,
                       bool              MipMaps        =  true)
    {
        Generate();
        Bind();


       // if( !m_Handle.GetID() )
       // {
       //     return false;
       // }

        //auto & I = m_Handle.__GetInfo( );

        glTexImage2D(
                     GL_TEXTURE_2D,
                     0,
                    static_cast<GLint>(InternalFormat),
                    size.x, size.y,
                    0,
                    static_cast<GLenum>(Format),
                    static_cast<GLenum>(Type),
                    Data);

        if(MipMaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

       // I.Format  = Format;
       // I.MipMaps = MipMaps;
       // I.Size    = size;
       // I.Type    = Type;
       // I.InternalFormat = InternalFormat;
        m_PixelDataType = Type;

        SetWrap( SamplerWrap::REPEAT, SamplerWrap::REPEAT);
        SetFilter(SamplerFilter::LINEAR_MIPMAP_LINEAR, SamplerFilter::LINEAR_MIPMAP_LINEAR);

        Unbind();
        return true;
    }

    void PasteSubImage( const gla::uvec2 & xy, const ImageBase & T, int level=0);


    /**
     *  Sets the Min and Mag filter for this texture
     *
     * @param Min The MIN filter to use
     * @param Mag The MAG filter to use
     */
    inline void SetFilter( SamplerFilter Min, SamplerFilter Mag)
    {
        Bind();

       // auto & I = m_Handle.__GetInfo();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLuint>(Min) );

        if( Mag == SamplerFilter::LINEAR_MIPMAP_LINEAR)  Mag = SamplerFilter::LINEAR;
        if( Mag == SamplerFilter::LINEAR_MIPMAP_NEAREST) Mag = SamplerFilter::NEAREST;

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLuint>(Mag));

        //I.MagFilter = Mag;
        //I.MinFilter = Mag;

        Unbind();
    }

    inline void SetWrap( SamplerWrap S_direction, SamplerWrap T_direction)
    {
        Bind();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLuint)S_direction);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLuint)T_direction);

        //I.S_Wrap = S_direction;
        //I.T_Wrap = T_direction;

        Unbind();
    }

    void SetActive( unsigned int unit)
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
        if(max!=0) return max;
        //glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
        return max;
    }

private:
    DataType m_PixelDataType;
};



inline void Sampler2D::PasteSubImage( const gla::uvec2 & xy, const ImageBase & T, int level)
{
    if( !Get() ) return;

    auto d = T.getRawData();
    if(!d) return;

    //have a bigger image bigImage and the
    // subImage sub
   // GLA_DOUT  << "Copying subimage over\n";
    Bind();

    // Note, i/j might have the origin int he bottom left corner instead of top left.
    GLenum Format[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
   // GLA_DOUT  << "PASTING SUB IMAGE: " << T.getChannels()-1 <<std::endl;
    glTexSubImage2D(GL_TEXTURE_2D,
                    level,
                    xy.x, xy.y,
                    T.size().x,
                    T.size().y,
                    Format[T.getChannels()-1],
                    (GLenum)m_PixelDataType,
                    d );

  //  GLA_DOUT  << "Error code: " << glGetError() << std::endl;

}

}}

#endif
