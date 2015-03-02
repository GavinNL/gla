#ifndef GLRE_TEXTURE_H
#define GLRE_TEXTURE_H

#include <glre/global.h>
#include <iostream>

namespace glre {

    class GPUTexture
    {
        public:
            typedef enum {
                NEAREST                = GL_NEAREST,
                LINEAR                 = GL_LINEAR,
                NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
                LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
                NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
                LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
            } FilterType;


            //===========================================================================
            // GL Texture Parameters
            //===========================================================================
            /**
             *  Sets the Min and Mag filter for this texture
             *
             * @param Min The MIN filter to use
             * @param Mag The MAG filter to use
             */
            inline void setFilter( FilterType Min, FilterType Mag)
            {
                bind();

                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Min);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mag);
            }



            inline void bind()
            {
                glBindTexture(GL_TEXTURE_2D, mTextureID);
            };


            /**
             *  Sets this texture to be the active texture for
             *  the.
             *
             *  @param unit The texture unit to set it as, usually between 0-32, but this is HW dependant. Default is 0.
             *
             */
            inline void setActiveTexture(unsigned int unit=0)
            {
                glActiveTexture(GL_TEXTURE0 + unit);
                glBindTexture(GL_TEXTURE_2D, mTextureID);
            }

            const uvec2 size() const { return mDim; }

            GLuint getID() { return mTextureID;  }

            uvec2        mDim;
            GLuint       mTextureID;
    };

    class Texture
    {

        public:




            Texture();
            Texture(const std::string & path);


            Texture(Texture && T)
            {
                mData = T.mData;
                mDim  = T.mDim;
                T.mDim = {0,0};
                T.mData = 0;
                std::cout << "Texture Move constructor\n";

            }

            Texture(Texture & T);

            Texture & operator=(Texture && T)
            {
                mData = T.mData;
                mDim  = T.mDim;
                T.mDim = {0,0};
                T.mData = 0;
                std::cout << "Texture Move Assignment\n";
                return *this;
            }

            /**
             * Frees all memory associated with this texture including GPU
             * data. The OpenGL Texture id will be freed.
             */
            ~Texture();




            //===========================================================================
            // Loading functions
            //===========================================================================
            /**
             *  Load an image from a raw buffer. The buffer is NOT raw pixels
             *  it is the binary data of an actual encoded image such as jpg or png
             *
             * @param Buffer Pointer to the buffer
             * @param buffersize The number of bytes in the buffer
             */
            void loadFromMemory( unsigned char * Buffer, int buffersize);

            /**
             *  Load an image from a file.
             *
             * @param Path a path to the file
             */
            void loadFromPath( const std::string & path);
            //===========================================================================


            GPUTexture CreateGPUTexture()
            {
                GPUTexture GPU;

                glGenTextures(1, &GPU.mTextureID);

                GPU.mDim = mDim;
                glBindTexture(GL_TEXTURE_2D, GPU.mTextureID);

                glTexImage2D( GL_TEXTURE_2D, 0 , GL_RGBA, mDim.x, mDim.y, 0, GL_BGRA, GL_UNSIGNED_BYTE,  (void*)mData );

                GPU.setFilter(GPUTexture::NEAREST, GPUTexture::NEAREST);

                return GPU;
            }

            /**
             * Copies a subimage from the CPU memory to the GPU memory
             */
            void updateGPU( const iRect & R = {0,0,-1,-1} );


            /**
             * Copies a subimage from the CPU of one texture to the GPU of this texture.
             */
            void updateGPU(const Texture & Tex, const iRect & R = {0,0,-1,-1} );


            /**
             * Clears the image data from the CPU. This does not clear the
             * texture data from the GPU. The OpenGL texture ID can still be used
             * until clearGPU() is called.
             */
            void clear();


            /**
             * Access pixel data on the CPU. This is unpredictable if you
             * have cleared the CPU data.
             */
            glre::ucol4 & operator ()(int x, int y) { return mData[ y*mDim[0] + x ]; }

            const glre::uvec2 & size() const { return mDim; };


            glre::ucol4* getRawData() const { return mData; };
        private:
            void _handleRawPixels(unsigned char * buffer, uint width, uint height);

            glre::ucol4 *mData;

            uvec2        mDim;



    };

}
#endif // TEXTURE_H
