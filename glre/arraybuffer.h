#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H

#include <vector>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

#include <iostream>

#include <glre/exceptions.h>

template<class T, GLenum GL_ARRAY_TARGET>
class ArrayBuffer
{
    public:
        ArrayBuffer() : mGLID(0), mGPUBufferSize(0), mGPUByteSize(0)
        {

        }

        ~ArrayBuffer()
        {
            clearGPU();
            clearCPU();
        }

        /**
         *  Sends the array buffer to the GPU. Once this is
         *  complete, you can clear the buffer from the CPU
         *
         */
        void sendToGPU()
        {
            if( mVector.size() == 0) return;

            glGenBuffers(1, &mGLID);

            if(!mGLID) throw glre::GPU_OUT_OF_MEMORY();

            glBindBuffer(GL_ARRAY_TARGET, mGLID);

            glBufferData(GL_ARRAY_TARGET,
                         cpuByteSize(),
                         &(mVector[0]),
                         GL_STATIC_DRAW);

            auto err = glGetError();

            if(err != 0)
            {
                throw glre::GPU_OUT_OF_MEMORY();

            } else {
                mGPUBufferSize = cpuBufferSize();
                mGPUByteSize   = cpuByteSize();
            }

        };

        /**
         *  Gets the number of items in the array buffer.
         *
         */
        inline std::size_t cpuBufferSize() const
        {
            return( mVector.size() );
        }

        /**
         *  Gets the number of items in the buffer on the GPU.
         *
         */
        inline std::size_t gpuBufferSize() const
        {
            return( mGPUBufferSize );
        }

        /**
         *  Gets the total number of bytes in the buffer
         *
         */
        inline std::size_t cpuByteSize() const
        {
            return( mVector.size() * sizeof(T) );
        }

        /**
         *  Gets the total number of bytes in the buffer
         *
         */
        inline std::size_t gpuByteSize() const
        {
            return( mGPUByteSize );
        }

        /**
         *  Inserts an item the back of the buffer.
         *
         */
        void insert(const T & v)
        {
            mVector.push_back(v);
        }

        /**
         *  Clears the buffer from the GPU. This does not automatically
         * clear the buffer from the CPU.
         *
         */
        void clearGPU()
        {
            if(mGLID) glDeleteBuffers(1, &mGLID);
            mGLID = 0;
        }

        /**
         *  Clears the buffer from the CPU leaving the GPU intact
         *
         */
        void clearCPU()
        {
            mVector.clear();
        }

        /**
         *  Binds the current buffer.
         *
         */
        inline void bind()
        {
            glBindBuffer(GL_ARRAY_TARGET, mGLID);
        };


        /**
         *  Access an element on the CPU buffer.
         *
         */
        T & operator[](int i){return mVector[i];}

        /**
         *  Gets the OpenGL id for the buffer.
         *
         */
        inline GLuint getGLHandle() { return mGLID; };

        /**
         *  Gets the reference to the CPU buffer.
         *
         */
        std::vector<T> & getBuffer() { return mVector; };

    public:
        std::vector<T> mVector;
        GLuint         mGLID;

        uint           mGPUBufferSize;
        uint           mGPUByteSize;
};

#endif // ARRAYBUFFER_H
