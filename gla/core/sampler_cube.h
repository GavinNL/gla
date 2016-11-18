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

#ifndef GLA_CORE_SAMPLERCUBE_H
#define GLA_CORE_SAMPLERCUBE_H

#include "types.h"
#include "handle.h"
#include "sampler2d.h"

namespace gla {

struct GenSamplerCube
{
    void operator()(GLuint & h)
    {
       glGenTextures(1, &h);
    }
};

struct DestSamplerCube
{
    void operator()(GLuint & h)
    {
        glDeleteTextures(1, &h);
    }
};

struct SamplerCubeData
{
    glm::uvec2  m_Size;
    std::size_t m_Depth;
    std::size_t m_MipmapCount;
    std::size_t m_Components;
};


    class SamplerCube;

    class SamplerCubeLayerRef
    {
        public:
            SamplerCubeLayerRef & operator=(const Image & img);
            SamplerCubeLayerRef(SamplerCube* R, GLenum f):ref(R),face(f){}

            SamplerCubeLayerRef & operator=(const SamplerCubeLayerRef & img) = delete;
        private:
            SamplerCube     *ref;
            GLenum     face;

            friend class SamplerCube;
    };

    class SamplerCube : public BaseHandle<GLuint, GenSamplerCube, DestSamplerCube, SamplerCubeData>
    {
        public:
            enum Face : GLenum
            {
                POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            };

            void Bind()
            {
                glBindTexture(GL_TEXTURE_CUBE_MAP, Get() );
            }
            void Unbind()
            {
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0 );
            }

            void Create(glm::uvec2 size, std::size_t components, int mipmaps);

            void SetFilter( SamplerFilter Min, SamplerFilter Mag);
            void SetWrap( SamplerWrap S_direction, SamplerWrap T_direction);
            void SetFace(const Image & T, Face face, const uvec2 & pOffset = uvec2(0, 0));

            void SetActive( unsigned int unit);

            SamplerCubeLayerRef operator[](Face face);

        private:



      //      SamplerCubeLayerRef m_LayerRef;
    };


    inline SamplerCubeLayerRef SamplerCube::operator[](Face face)
    {
        return SamplerCubeLayerRef(this,face);
    }

    inline SamplerCubeLayerRef & SamplerCubeLayerRef::operator=(const Image & img)
    {
        ref->SetFace( img , static_cast<SamplerCube::Face>(face) );
        return *this;
    }

    inline void SamplerCube::SetFace(const Image & T, Face face, const uvec2 & pOffset)
    {
        Bind();

        if( Get() == 0)
        {
            throw std::runtime_error("Error: SamplerCube not generated\n");
        }

        auto mSize       = SharedData().m_Size;
        auto mComponents = SharedData().m_Components;
        auto m_Depth     = SharedData().m_Depth;

        if( T.size().x + pOffset.x > mSize.x || T.size().y + pOffset.y > mSize.y )
        {
            throw std::runtime_error("ERROR: The Image is too large for the SamplerCube");
        }

        if( T.getChannels() != mComponents )
        {
            char msg[256];
            sprintf(msg, "ERROR: Texture and TextureArray have missmatched components, Texture = %d   Texture Array = %d", (int)mComponents, (int)T.getChannels() );
            throw std::runtime_error(std::string(msg) );
        }

        GLuint format[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};




        glTexSubImage2D(  static_cast<GLenum>(face),
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

    inline void SamplerCube::Create(glm::uvec2 size, std::size_t components, int mipmaps)
    {
        Release();
        Generate();

        if(!Get())
        {
            throw std::runtime_error("ERROR: Error generating Handle for SamplerCube");
        }

        Bind();

        GLuint format[4] = {GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};

        glTexStorage2D( GL_TEXTURE_CUBE_MAP, mipmaps, format[components-1], size.x, size.y );
        //glTexStorage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, mipmaps, format[components-1], size.x, size.y );
        //glTexStorage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, mipmaps, format[components-1], size.x, size.y );
        //glTexStorage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, mipmaps, format[components-1], size.x, size.y );
        //glTexStorage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, mipmaps, format[components-1], size.x, size.y );
        //glTexStorage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, mipmaps, format[components-1], size.x, size.y );

        std::cout << "ERROR: " << std::hex << glGetError() << std::endl;
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format[components-1], size.x, size.y , 0, format[components-1], GL_UNSIGNED_BYTE, 0);
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format[components-1], size.x, size.y , 0, format[components-1], GL_UNSIGNED_BYTE, 0);
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format[components-1], size.x, size.y , 0, format[components-1], GL_UNSIGNED_BYTE, 0);
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format[components-1], size.x, size.y , 0, format[components-1], GL_UNSIGNED_BYTE, 0);
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format[components-1], size.x, size.y , 0, format[components-1], GL_UNSIGNED_BYTE, 0);
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format[components-1], size.x, size.y , 0, format[components-1], GL_UNSIGNED_BYTE, 0);

        SharedData().m_Size        = size;
        SharedData().m_MipmapCount = mipmaps;
        SharedData().m_Components  = components;

        SetFilter( SamplerFilter::LINEAR_MIPMAP_LINEAR, SamplerFilter::LINEAR_MIPMAP_LINEAR);
        SetWrap(SamplerWrap::REPEAT, SamplerWrap::REPEAT);

        glGenerateMipmap( GL_TEXTURE_CUBE_MAP);

    }

    inline void SamplerCube::SetFilter( SamplerFilter Min, SamplerFilter Mag)
    {
        Bind();

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLuint>(Min) );

        if( Mag == SamplerFilter::LINEAR_MIPMAP_LINEAR)  Mag = SamplerFilter::LINEAR;
        if( Mag == SamplerFilter::LINEAR_MIPMAP_NEAREST) Mag = SamplerFilter::NEAREST;

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLuint>(Mag));

        Unbind();
    }

    inline void SamplerCube::SetWrap( SamplerWrap S_direction, SamplerWrap T_direction)
    {
        Bind();

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLuint)S_direction);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLuint)T_direction);

        //I.S_Wrap = S_direction;
        //I.T_Wrap = T_direction;

        Unbind();
    }

    inline void SamplerCube::SetActive( unsigned int unit)
    {
        glActiveTexture(GL_TEXTURE0+unit);
        Bind();
    }
}



#endif
