#ifndef EXP_SAMPLER2DARRAY
#define EXP_SAMPLER2DARRAY

#include "types.h"
#include "handle.h"
#include "sampler2d.h"

namespace gla { namespace experimental {

struct GenSampler2DArray
{
    void operator()(GLuint & h)
    {
       glGenTextures(1, &h);
    }
};

struct DestSampler2DArray
{
    void operator()(GLuint & h)
    {
        glDeleteTextures(1, &h);
    }
};


    class Sampler2DArray;

    class Sampler2DArrayLayerRef
    {
        public:
            Sampler2DArrayLayerRef & operator=(const Image & img);
        private:
            Sampler2DArray *ref;
            std::size_t     layer;

            friend class Sampler2DArray;
    };

    class Sampler2DArray : public BaseHandle<GLuint, GenSampler2DArray, DestSampler2DArray>
    {
        public:

            void Bind()
            {
                glBindTexture(GL_TEXTURE_2D_ARRAY, Get() );
            }
            void Unbind()
            {
                glBindTexture(GL_TEXTURE_2D_ARRAY, 0 );
            }

            void Create(glm::uvec2 size, std::size_t depth, std::size_t components, int mipmaps);

            void SetFilter( SamplerFilter Min, SamplerFilter Mag);
            void SetWrap( SamplerWrap S_direction, SamplerWrap T_direction);
            void SetLayer(const Image & T, uint Layer, const uvec2 & pOffset=uvec2(0,0));

            void SetActive( unsigned int unit);

            Sampler2DArrayLayerRef & operator[](int layer);

        private:

            glm::uvec2  m_Size;
            std::size_t m_Depth;
            std::size_t m_MipmapCount;
            std::size_t m_Components;

            Sampler2DArrayLayerRef m_LayerRef;
    };


    inline Sampler2DArrayLayerRef & Sampler2DArray::operator[](int layer)
    {
        m_LayerRef.ref   = this;
        m_LayerRef.layer = layer;
        return m_LayerRef;
    }

    inline Sampler2DArrayLayerRef & Sampler2DArrayLayerRef::operator=(const Image & img)
    {
        ref->SetLayer( img , layer );
        return *this;
    }

    inline void Sampler2DArray::SetLayer(const Image & T, uint Layer, const uvec2 & pOffset )
    {
        Bind();

        if( Get() == 0)
        {
            throw std::runtime_error("Error: Sampler2DArray not generated\n");
        }

        auto mSize       = m_Size;
        auto mComponents = m_Components;

        if( Layer >= m_Depth)
        {
            throw std::runtime_error("ERROR: Layer number is larger than the allocated size of the TextureArray.");
        }

        if( T.size().x + pOffset.x > mSize.x || T.size().y + pOffset.y > mSize.y )
        {
            throw std::runtime_error("ERROR: The Image is too large for the Sampler2DArray");
        }

        if( T.getChannels() != mComponents )
        {
            char msg[256];
            sprintf(msg, "ERROR: Texture and TextureArray have missmatched components, Texture = %d   Texture Array = %d", (int)mComponents, (int)T.getChannels() );
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

    inline void Sampler2DArray::Create(glm::uvec2 size, std::size_t depth, std::size_t components, int mipmaps)
    {
        Release();
        Generate();
        if(!Get())
        {
            throw std::runtime_error("ERROR: Error generating Handle for Sampler2DArray");
        }

        Bind();

        GLuint format[4] = {GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};
        glTexStorage3D( GL_TEXTURE_2D_ARRAY, mipmaps, format[components-1], size.x, size.y, depth);

        m_Size        = size;
        m_Depth       = depth;
        m_MipmapCount = mipmaps;
        m_Components  = components;

        SetFilter( SamplerFilter::LINEAR_MIPMAP_LINEAR, SamplerFilter::LINEAR_MIPMAP_LINEAR);
        SetWrap(SamplerWrap::REPEAT, SamplerWrap::REPEAT);

        glGenerateMipmap( GL_TEXTURE_2D_ARRAY);
    }

    inline void Sampler2DArray::SetFilter( SamplerFilter Min, SamplerFilter Mag)
    {
        Bind();

       // auto & I = m_Handle.__GetInfo();

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLuint>(Min) );

        if( Mag == SamplerFilter::LINEAR_MIPMAP_LINEAR)  Mag = SamplerFilter::LINEAR;
        if( Mag == SamplerFilter::LINEAR_MIPMAP_NEAREST) Mag = SamplerFilter::NEAREST;

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLuint>(Mag));

        //I.MagFilter = Mag;
        //I.MinFilter = Mag;

        Unbind();
    }

    inline void Sampler2DArray::SetWrap( SamplerWrap S_direction, SamplerWrap T_direction)
    {
        Bind();

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, (GLuint)S_direction);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, (GLuint)T_direction);

        //I.S_Wrap = S_direction;
        //I.T_Wrap = T_direction;

        Unbind();
    }

    inline void Sampler2DArray::SetActive( unsigned int unit)
    {
        glActiveTexture(GL_TEXTURE0+unit);
        Bind();
        ////static unsigned int CurrentlyBoundTextureUnits[128] = {0};
        //auto CurrentlyBoundTextureUnits = gla::GetActiveTextures();
        //
        //if( CurrentlyBoundTextureUnits[unit] != m_Handle.GetID() )
        //{
        //    glActiveTexture(GL_TEXTURE0+unit);
        //    m_Handle.Bind();
        //    CurrentlyBoundTextureUnits[unit] = m_Handle.GetID();
        //}
    }
}

}

#endif
