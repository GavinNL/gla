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

#ifndef EXP_SAMPLER2DARRAY
#define EXP_SAMPLER2DARRAY

#include "types.h"
#include "handle.h"
#include "sampler2d.h"

namespace gla {

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

struct Sampler2DArrayData
{
    glm::uvec2  m_Size;
    std::size_t m_Depth;
    std::size_t m_MipmapCount;
    std::size_t m_Components;
};


    class Sampler2DArray;

    class Sampler2DArrayLayerRef
    {
        public:
            Sampler2DArrayLayerRef & operator=(const Image & img);
            Sampler2DArrayLayerRef(Sampler2DArray* R, std::size_t l):ref(R),layer(l){}

            Sampler2DArrayLayerRef & operator=(const Sampler2DArrayLayerRef & img) = delete;
        private:
            Sampler2DArray *ref;
            std::size_t     layer;

            friend class Sampler2DArray;
    };

    class Sampler2DArray : public BaseHandle<GLuint, GenSampler2DArray, DestSampler2DArray, Sampler2DArrayData>
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
			void SetLayer(const Image & T, std::uint32_t Layer, const uvec2 & pOffset = uvec2(0, 0));

            void SetActive( unsigned int unit);

            Sampler2DArrayLayerRef operator[](int layer);

        private:



      //      Sampler2DArrayLayerRef m_LayerRef;
    };


    inline Sampler2DArrayLayerRef Sampler2DArray::operator[](int layer)
    {
        return Sampler2DArrayLayerRef(this,layer);
    }

    inline Sampler2DArrayLayerRef & Sampler2DArrayLayerRef::operator=(const Image & img)
    {
        ref->SetLayer( img , static_cast<std::uint32_t>(layer) );
        return *this;
    }

    inline void Sampler2DArray::SetLayer(const Image & T, std::uint32_t Layer, const uvec2 & pOffset)
    {
        Bind();

        if( Get() == 0)
        {
            throw std::runtime_error("Error: Sampler2DArray not generated\n");
        }

        auto mSize       = SharedData().m_Size;
        auto mComponents = SharedData().m_Components;
        auto m_Depth     = SharedData().m_Depth;

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
        glTexStorage3D( GL_TEXTURE_2D_ARRAY, mipmaps, format[components-1], size.x, size.y, static_cast<GLsizei>(depth) );

        SharedData().m_Size        = size;
        SharedData().m_Depth       = depth;
        SharedData().m_MipmapCount = mipmaps;
        SharedData().m_Components  = components;

        SetFilter( SamplerFilter::LINEAR_MIPMAP_LINEAR, SamplerFilter::LINEAR_MIPMAP_LINEAR);
        SetWrap(SamplerWrap::REPEAT, SamplerWrap::REPEAT);

        glGenerateMipmap( GL_TEXTURE_2D_ARRAY);
    }

    inline void Sampler2DArray::SetFilter( SamplerFilter Min, SamplerFilter Mag)
    {
        Bind();

       // auto & I = m_Handle.__GetInfo();

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLuint>(Min) );

        if( Mag == SamplerFilter::LINEAR_MIPMAP_LINEAR)  Mag = SamplerFilter::LINEAR;
        if( Mag == SamplerFilter::LINEAR_MIPMAP_NEAREST) Mag = SamplerFilter::NEAREST;

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLuint>(Mag));

        //I.MagFilter = Mag;
        //I.MinFilter = Mag;

        Unbind();
    }

    inline void Sampler2DArray::SetWrap( SamplerWrap S_direction, SamplerWrap T_direction)
    {
        Bind();

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, (GLuint)S_direction);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, (GLuint)T_direction);

        //I.S_Wrap = S_direction;
        //I.T_Wrap = T_direction;

        Unbind();
    }

    inline void Sampler2DArray::SetActive( unsigned int unit)
    {
        glActiveTexture(GL_TEXTURE0+unit);
        Bind();
    }
}



#endif
