#ifndef GLA_GPUTEXTUREARRAY_H
#define GLA_GPUTEXTUREARRAY_H

#include <gla/types.h>
#include <memory>
#include <gla/texture.h>

namespace gla {


///struct GPUTextureArrayInfo
///{
///    uvec3        size;
///    int          mipmaps;
///    unsigned int channels;
///};

//===========================================================================
// GPUTextureArray
//   - an array of 2D textures stored on the GPU.
//===========================================================================

struct TexArrayInfo
{
    unsigned int UseCount   = 0;
    uvec3        Size       = {0,0,0};
    unsigned int Channels   = 0;
    unsigned int MipMaps    = 0;
    TexFilter    MinFilter;
    TexFilter    MagFilter;
    TexWrap      S_Wrap;
    TexWrap      T_Wrap;
};

struct TexArrayHandler
{
    inline static void Create  (GLuint & h) { glGenTextures(1, &h); }
    inline static void Release (GLuint & h) { glDeleteTextures(1, &h); }
    inline static void Bind    (GLuint & h) { glBindTexture(GL_TEXTURE_2D_ARRAY, h); }
    inline static void Unbind  (GLuint & h) { glBindTexture(GL_TEXTURE_2D_ARRAY, 0);  }
};

class GPUTextureArray
{

public:
    using HandleType = Handle<GLuint, TexArrayHandler, TexArrayInfo>;

    HandleType m_Handle;

    void Bind()     { m_Handle.Bind();    }
    void Unbind()   { m_Handle.Unbind();  }
    void Release()  { m_Handle.Release(); }

    /**
     * @brief create Creates the texture array on the GPU
     * @param size the width and height dimensions of the textures
     * @param depth the number of textures to allocate space for
     * @param MipMapCount the mipmap level, default is 1
     */
    void Create(uvec2 size, unsigned int depth, unsigned int components=4, int MipMapCount=1);



    /**
     * @brief SetLayer - Copies a CPU texture into a particular depth in the Texture Array
     * @param T - CPU texture to copy. This must be the same dimensions as the TextureArray
     * @param Layer - the layer number to copy the texture into
     * @param pOffset - an offset parameter.
     */
    void SetLayer(const Texture & T, uint Layer, const uvec2 & pOffset=uvec2(0,0))
    {
        m_Handle.Bind();

        assert(m_Handle.GetID() != 0);

        auto mSize       = m_Handle.GetInfo().Size;
        auto mComponents = m_Handle.GetInfo().Channels;

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
            throw std::runtime_error(std::string(msg) );
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


    /**
     *  Sets the Min and Mag filter for this texture
     *
     * @param Min The MIN filter to use
     * @param Mag The MAG filter to use
     */
    inline void SetFilter( TexFilter Min, TexFilter Mag)
    {
        m_Handle.Bind();
        assert(m_Handle.GetID() != 0);

        auto & I = m_Handle.__GetInfo();

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, (GLuint)Min);

        if( Mag == TexFilter::LINEAR_MIPMAP_LINEAR)  Mag = TexFilter::LINEAR;
        if( Mag == TexFilter::LINEAR_MIPMAP_NEAREST) Mag = TexFilter::NEAREST;

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, (GLuint)Mag);

        I.MagFilter = Mag;
        I.MinFilter = Mag;
    }

    inline void SetTextureWrap( TexWrap S_direction, TexWrap T_direction)
    {
        m_Handle.Bind();
        assert(m_Handle.GetID()!=0);

        auto & I = m_Handle.__GetInfo();

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, (GLuint)S_direction);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, (GLuint)T_direction);

        I.S_Wrap = S_direction;
        I.T_Wrap = T_direction;
    }

//    inline void SetActive(unsigned int unit=0)
//    {
//        glActiveTexture(GL_TEXTURE0 + unit);
//        //glBindTexture(GL_TEXTURE_2D_ARRAY, mID);
//        bind();

//    }

    void SetActive( unsigned int unit)
    {
        //static unsigned int CurrentlyBoundTextureUnits[128] = {0};
        auto CurrentlyBoundTextureUnits = gla::GetActiveTextures();

        if( CurrentlyBoundTextureUnits[unit] != m_Handle.GetID() )
        {
            glActiveTexture(GL_TEXTURE0+unit);
            m_Handle.Bind();
            CurrentlyBoundTextureUnits[unit] = m_Handle.GetID();
        }
    }



    //inline void bindToUnit(unsigned int unit=0)
    //{
    //    glActiveTexture(GL_TEXTURE0 + unit);
    //    glBindTexture(GL_TEXTURE_2D_ARRAY, mID);
    //}

    //inline GLuint getID() const { return mID;   }

    //inline const GPUTextureArrayInfo Info() { return mInfo!=0 ? *mInfo.get() : GPUTextureArrayInfo(); }
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

        //std::shared_ptr<GPUTextureArrayInfo> mInfo;
};



//============================================
inline void GPUTextureArray::Create(uvec2 size, unsigned int depth, unsigned int components, int MipMapCount)
{
    try
    {
        GetGLError();
    } catch (std::exception & e)
    {

    }

    //if( mID ) clear();
    m_Handle.Release();
    m_Handle.Create();
    //glGenTextures(1, &mID);

    GetGLError();
    //printf("Generated textures\n");
    if( !m_Handle.GetID() )
    {
        throw gla::GLA_EXCEPTION("ERROR: error generating textures for TextureArray2D");
    }

    m_Handle.Bind();

    GLuint format[4] = {GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};
    //glTexStorage3D( GL_TEXTURE_2D_ARRAY, MipMapCount, format[components], size.x, size.y, depth);
    glTexStorage3D( GL_TEXTURE_2D_ARRAY, MipMapCount, format[components-1], size.x, size.y, depth);

    GetGLError();

    //printf("glTexStorage complete\n");
    assert(m_Handle.GetID() != 0 );

    auto & Info   = m_Handle.__GetInfo();

    Info.Channels = components;
    Info.Size     = uvec3(size.x, size.y, depth);
    Info.MipMaps  = MipMapCount;

    glGenerateMipmap( GL_TEXTURE_2D_ARRAY );

    SetFilter( TexFilter::LINEAR_MIPMAP_LINEAR, TexFilter::LINEAR_MIPMAP_LINEAR);
    SetTextureWrap(TexWrap::REPEAT, TexWrap::REPEAT);


    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S    , GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T    , GL_REPEAT);

    GetGLError();

    //mSize       = uvec3(size.x, size.y, depth);
    //mComponents = components;

    //auto id = mID;
    //mInfo = std::shared_ptr<GPUTextureArrayInfo>( new GPUTextureArrayInfo, [=](GPUTextureArrayInfo* a){ delete a; glDeleteTextures(1, &id); GLA_DOUT  << "Deleting Texture Array: " << id << std::endl; } );

};




}

#endif // GPUTEXTUREARRAY_H
