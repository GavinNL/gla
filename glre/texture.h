#ifndef TEXTURE_H
#define TEXTURE_H

#include <glre/global.h>
#include <glre/stb/stb_image.h>

namespace glre {

    class Texture
    {
        public:
            Texture();

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
