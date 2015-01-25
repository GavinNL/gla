#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H

#include <vector>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>


template<class T, GLenum GL_ARRAY_TARGET>
class ArrayBuffer
{
    public:
        ArrayBuffer() : mGLID(0)
        {

        }

        void sendToGPU()
        {
            if( mVector.size() == 0) return;

            glGenBuffers(1, &mGLID);
            glBindBuffer(GL_ARRAY_TARGET, mGLID);

            glBufferData(GL_ARRAY_TARGET,
                         byteSize(),
                         &(mVector[0]),
                         GL_STATIC_DRAW);
        };

        inline std::size_t size()
        {
            return( mVector.size() );
        }

        inline std::size_t byteSize()
        {
            return( mVector.size() * sizeof(T) );
        }

        void insert(const T & v)
        {
            mVector.push_back(v);
        }

        void clearGPU()
        {
            if(mGLID) glDeleteBuffers(1, &mGLID);
            mGLID = 0;
        }

        void clearCPU()
        {
            mVector.clear();
        }

        inline void bind()
        {
            glBindBuffer(GL_ARRAY_TARGET, mGLID);
        };

        GLuint getGLHandle() { return mGLID; };

        std::vector<T> & getBuffer() { return mVector; };

    public:
        std::vector<T> mVector;
        GLuint         mGLID;
};

#endif // ARRAYBUFFER_H
