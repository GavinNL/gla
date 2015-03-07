#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H

#include <vector>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glre/exceptions.h>

namespace glre {


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
            std::cout << "Array buffer deleted\n";
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

            //if(!mGLID) throw glre::GPU_OUT_OF_MEMORY();

            glBindBuffer(GL_ARRAY_TARGET, mGLID);

            glBufferData(GL_ARRAY_TARGET,
                         cpuByteSize(),
                         &(mVector[0]),
                         GL_STATIC_DRAW);

            auto err = glGetError();

            if(err != 0)
            {
                throw glre::GLRE_EXCEPTION("Error sending ArrayBuffer to the GPU. Did you create an OpenGL rendering context fist?");

            } else {
                mGPUBufferSize = cpuBufferSize();
                mGPUByteSize   = cpuByteSize();
            }
            std::cout << "Array Buffer sent to GPU. ID: " << mGLID << "\n ";

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
            mGPUBufferSize = mGPUByteSize = 0;
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
        inline GLuint getID() { return mGLID; };

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

}



//===================================================================================

namespace glre {

enum ARRAY_TYPE {
    ARRAY_BUFFER                         = GL_ARRAY_BUFFER,
    ELEMENT_ARRAY_BUFFER                 = GL_ELEMENT_ARRAY_BUFFER
};


class GPUArrayBuffer
{
    public:
        void bind() { glBindBuffer(mArrayType, mGLID); } ;

        inline GLuint getArrayType(){ return mArrayType;   };
        inline GLuint getID()       { return mGLID;        };
        inline uint   getByteSize() { return mSizeInBytes; };


        GLuint         mGLID;             // The GL ID associated with it.
        ARRAY_TYPE     mArrayType;        // The type of buffer, either GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
        unsigned int   mSizeInBytes;      // Size in bytes of the buffer
};

class ArrayBuffer_b
{
    /* Creates a GPUArray as a specifc type, either ARRAY_BUFFER (used for vertex info), or ELEMENT_ARRAY_BUFFER (used for index info)
     */
    public:
        GPUArrayBuffer createGPUObject( ARRAY_TYPE TY )
        {
            GPUArrayBuffer   B;

            glGenBuffers(1,         &B.mGLID);
            glBindBuffer(TY, B.mGLID);
            glBufferData(TY,
                         this->getByteSize(),
                         this->getData(),
                         GL_STATIC_DRAW);



            auto err = glGetError();
            if(err != 0)
            {
                throw glre::GLRE_EXCEPTION("Error sending ArrayBuffer to the GPU. Did you create an OpenGL rendering context fist?");

            }

            B.mSizeInBytes = this->getByteSize();
            B.mArrayType   = TY;

            std::cout << "================Buffer created=================\n";
            std::cout << "ID        : " << B.mGLID <<  std::endl;;
            std::cout << "arraytype : " << (B.mArrayType==GL_ARRAY_BUFFER ? std::string("ARRAY") : std::string("ELEMENT")) <<  std::endl;
            std::cout << "bytesize  : " << B.mSizeInBytes <<  std::endl;
            std::cout << "vertcount : " << this->getVertexCount() <<  std::endl;
            std::cout << "val/ver   : " << this->getValuesPerVertex() <<  std::endl;
            std::cout << "===============================================\n";
            return B;
        }

        GLuint  getID() { return mGLID; }

        virtual GLenum getIntegralType()    const = 0;
        virtual void*  getData()            const = 0;
        virtual uint   getByteSize()        const = 0;
        virtual uint   getVertexCount()     const = 0;
        virtual uint   getValuesPerVertex() const = 0;

        bool sameTypeAs( const ArrayBuffer_b & b)
        {
            return( this->getIntegralType()==b.getIntegralType() && this->getValuesPerVertex()==b.getValuesPerVertex());
        }

    GLuint mGLID;
    GLuint mArrayType;
    uint   mBufferSize;

};


template <class T, GLenum INTEGRAL_TYPE>
class ArrayBuffer_T : public ArrayBuffer_b
{
public:
    virtual uint  getVertexCount() const { return (uint)mVector.size(); };
    virtual void* getData()        const { return (void*)mVector.data(); };

    virtual uint  getByteSize() const
    {
        return( (uint)(mVector.size() * sizeof(T)) );
    };

    inline virtual GLenum getIntegralType() const
    {
        return INTEGRAL_TYPE;
    }

    inline virtual uint getValuesPerVertex() const
    {
        switch(INTEGRAL_TYPE)
        {
            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
                return sizeof(T);
            case GL_SHORT:
            case GL_2_BYTES:
            case GL_UNSIGNED_SHORT:
                return sizeof(T)/2;
            case GL_3_BYTES:
                return sizeof(T)/3;
            case GL_INT:
            case GL_UNSIGNED_INT:
            case GL_FLOAT:
            case GL_4_BYTES:
                return sizeof(T)/4;
        }
    }

    void insert(const T & v) { mVector.push_back(v);  };
    void clear() { mVector.clear(); };
    void addOffset(const T & offsetValue)
    {
        for(int i=0;i<mVector.size();i++) mVector[i] += offsetValue;
    }

    T & getVertex(int i) { return mVector[i]; };

    private:
        std::vector<T> mVector;

};


}

#endif // ARRAYBUFFER_H
