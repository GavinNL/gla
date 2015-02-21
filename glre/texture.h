#ifndef TEXTURE_H
#define TEXTURE_H

#include <glre/global.h>

namespace glre {

    class Texture
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


            Texture();
            Texture(const std::string & path, bool SendToGPU=false);

            /**
             * Frees all memory associated with this texture including GPU
             * data. The OpenGL Texture id will be freed.
             */
            ~Texture();

            inline GLint getID() const
            {
                return mTextureID;
            }


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

            //===========================================================================


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

            /**
             *  Bind the texture making any modifications applied to this texture.
             *
             */
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

            /**
             * Sends the image data to the GPU to be used
             * by OpenGL
             */
            void sendToGPU();

            /**
             * Clears the texture from GPU memory. The image will still be in
             * CPU memory unless it was explicitly cleared from the CPU using
             * clearCPU()
             */
            void clearGPU();

            /**
             * Clears the image data from the CPU. This does not clear the
             * texture data from the GPU. The OpenGL texture ID can still be used
             * until clearGPU() is called.
             */
            void clearCPU();

            /**
             * Access pixel data on the CPU. This is unpredictable if you
             * have cleared the CPU data.
             */
            glre::ucol4 & operator ()(int x, int y) { return mData[ y*mWidth + x ]; }

        private:
            void _handleRawPixels(unsigned char * buffer, uint width, uint height);

            glre::ucol4 *mData;

            uint         mWidth;
            uint         mHeight;

            GLuint       mTextureID;

    };

}
#endif // TEXTURE_H
