#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H

#include <vector>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <gla/core/exceptions.h>
#include <gla/core/types.h>
#include <memory>


/*
namespace gla {

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
                throw gla::GLA_EXCEPTION("Error sending ArrayBuffer to the GPU. Did you create an OpenGL rendering context fist?");

            } else {
                mGPUBufferSize = cpuBufferSize();
                mGPUByteSize   = cpuByteSize();
            }
            //std::cout << "Array Buffer sent to GPU. ID: " << mGLID << "\n ";

        };



        inline std::size_t cpuBufferSize() const
        {
            return( mVector.size() );
        }


        inline std::size_t gpuBufferSize() const
        {
            return( mGPUBufferSize );
        }


        inline std::size_t cpuByteSize() const
        {
            return( mVector.size() * sizeof(T) );
        }


        inline std::size_t gpuByteSize() const
        {
            return( mGPUByteSize );
        }


        void insert(const T & v)
        {
            mVector.push_back(v);
        }


        void clearGPU()
        {
            if(mGLID) glDeleteBuffers(1, &mGLID);
            mGLID = 0;
            mGPUBufferSize = mGPUByteSize = 0;
        }


        void clearCPU()
        {
            mVector.clear();
        }


        inline void bind()
        {
            glBindBuffer(GL_ARRAY_TARGET, mGLID);
        };



        T & operator[](int i){return mVector[i];}


        inline GLuint getID() { return mGLID; };


        std::vector<T> & getBuffer() { return mVector; };

    public:
        std::vector<T> mVector;
        GLuint         mGLID;

        uint           mGPUBufferSize;
        uint           mGPUByteSize;
};

*/


//===================================================================================

namespace gla {

struct ArrayBufferInfo
{
    BUFFER_ELEMENT_TYPE    mBufferType;
    unsigned int           mNumberOfItems;
};

class GPUArrayBuffer
{
    public:
    /**
         * @brief bind binds the buffer as the current buffer on the GPU
         * @param type The type of buffer to bind it as, either ARRAY_BUFFER or ELEMENT_ARRAY_BUFFER
         */
        inline void      bind(ARRAY_TYPE type) const { glBindBuffer(type, mGLID); } ;

        /**
         * @brief getID
         * @return The openGL id for the array buffer.
         */
        inline GLuint    getID()       { return mGLID;        };


        /**
         * @brief getByteSize
         * @return The size of the buffer in bytes.
         */
        inline uint   getByteSize()
        {
           // return mSizeInBytes;

            int size=0;
            switch( IntegralType() )
            {
                case GL_FLOAT:
                case GL_UNSIGNED_INT:
                case GL_INT:
                    size = 4; break;

                case GL_SHORT:
                case GL_UNSIGNED_SHORT:
                    size = 2; break;

                case GL_BYTE:
                case GL_UNSIGNED_BYTE:
                    size = 1; break;

            }

            return mNumberOfItems * ElementsPerItem() * size;
        };


        /**
         * @brief EnableAttribute
         * @param index The index of the attribute to set this buffer as
         * @param NormaliseVector set to true if you want to normalize the vector values. Good for surface Normals
         */
        inline void EnableAttribute(int index, bool NormaliseVector=false) const
        {
            // Bind it as an array buffer since it probably wont be an element buffer
            // if we are using attributes.
            bind(ARRAY_BUFFER);
            glEnableVertexAttribArray(index);

            //const GLenum inttype[5] = {GL_FLOAT, GL_INT, GL_UNSIGNED_INT, GL_BYTE, GL_SHORT};
            //const char IntegralTypeS[5][50] = {"GL_FLOAT", "GL_INT", "GL_UNSIGNED_INT", "GL_BYTE", "GL_SHORT", "GL_UNSIGNED_BYTE"};
            //printf("Activating Attirubte (%d) as %d(%d) elements of type %s\n", index, ((int)mBufferType)%4+1, ((int)mBufferType),IntegralTypeS[(int)mBufferType/5]);


            glVertexAttribPointer(index,
                                  ElementsPerItem(),
                                  IntegralType(),  // GL_FLOAT/GL_INT/etc
                                  NormaliseVector,                   // Should we normlize the vector?
                                  0,  // stride
                                  0); // buffer offset
        }

        /**
         * @brief DisableAttribute
         * @param index the index number to disable
         */
        static inline void DisableAttribute(int index)
        {
            glDisableVertexAttribArray(index);
        }


        /**
         * @brief clear Sets the Arraybuffer to be flagged for deletion.
         */
        void clear()
        {
           // glDeleteBuffers(1, &mGLID);
            mGLID = 0;
            mNumberOfItems = 0;
            mInfo = 0; // The shared pointer will automatically delete the buffer off the GPU
        }

        /**
         * @brief Render
         * @param p The Primitave type to render as, TRIANGLES, QUADS, etc
         */
        void Render(PRIMITAVE p)
        {
           glDrawArrays( p, 0,  NumberOfItems() );
        }

        void Render(PRIMITAVE p, int numberofitems)
        {
           glDrawArrays( p, 0,  numberofitems );
        }

        void Render(PRIMITAVE p, int start, int numberofitems)
        {
           glDrawArrays( p, start,  numberofitems );
        }

        /**
         * @brief NumberOfItems
         * @return Returns the number of items in the buffer.
         *
         * If you have 4 values of vec3s, it returns 4, not 12.
         */
        int NumberOfItems() const
        {
            return mNumberOfItems;
        }

        /**
         * @brief ElementsPerItem
         * @return Returns the number of elements per item, eg: Vec3s will return 3, vec4 will return 4
         */
        int ElementsPerItem() const
        {
            return (((int)mBufferType)%4+1);
        }

        /**
         * @brief IntegralType
         * @return Returns the openGL integral type, GL_FLOAT, GL_INT, etc
         */
        GLenum IntegralType() const
        {
            const GLenum IntegralType[7] = {GL_FLOAT, GL_INT, GL_UNSIGNED_INT, GL_BYTE, GL_SHORT, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT};
            return( IntegralType[(int)mBufferType/7] );
        }

        GLuint                 mGLID;             // The GL ID associated with it.
        BUFFER_ELEMENT_TYPE    mBufferType;
        unsigned int           mNumberOfItems;

        std::shared_ptr<ArrayBufferInfo> mInfo;
};

class ArrayBuffer_b
{
    /* Creates a GPUArray as a specifc type, either ARRAY_BUFFER (used for vertex info), or ELEMENT_ARRAY_BUFFER (used for index info)
     */
    public:

        GPUArrayBuffer toGPU( ARRAY_TYPE TY )
        {
            GPUArrayBuffer   B;

            glGenBuffers(1, &B.mGLID);
            glBindBuffer(TY, B.mGLID);
            glBufferData(TY,
                         this->getByteSize(),
                         this->getData(),
                         GL_STATIC_DRAW);


            int id = B.mGLID;
            B.mInfo = std::shared_ptr<ArrayBufferInfo>( new ArrayBufferInfo, [=](ArrayBufferInfo* a){ delete a; glDeleteProgram(id); std::cout << "Deleting ArrayBuffer: " << id << std::endl; } );

            auto err = glGetError();
            if(err != 0)
            {
                throw gla::GLA_EXCEPTION("Error sending ArrayBuffer to the GPU. Did you create an OpenGL rendering context fist?");

            }

            //B.mSizeInBytes = this->getByteSize();
            B.mNumberOfItems = getVertexCount();
            B.mBufferType  = mBufferElementType;
            //B.mArrayType   = TY;


#ifdef GLA_VERBOSE_BUFFERS
            std::cout << "================Buffer created=================\n";
            std::cout << "ID        : " << B.mGLID <<  std::endl;
            std::cout << "bytesize  : " << B.getByteSize() <<  std::endl;
            std::cout << "vertcount : " << this->getVertexCount() <<  std::endl;
            std::cout << "val/ver   : " << this->getValuesPerVertex() <<  std::endl;
            std::cout << "====================S===========================\n";
#endif
            return B;
        }

        GLuint  getID() { return mGLID; }

        //virtual FUNDAMENTAL_TYPE getIntegralType()    const = 0;
        virtual void*            getData()            const = 0;
        virtual uint             getByteSize()        const = 0;
        virtual uint             getVertexCount()     const = 0;
        virtual uint             getValuesPerVertex() const = 0;

        /**
         * Determines if two buffers are of the same type. Two buffers are of the same type if
         * their fundamental type (float, byte, unsigned int, etc) are the same.
         * And the number of values per vertex are the same (eg: they're both 2D vertices, or 3D vertices).
         *
         * @param b A const reference to another buffer.
         */
        bool sameTypeAs( const ArrayBuffer_b & b)
        {
            return( this->mBufferElementType == b.mBufferElementType );
            //return( this->getIntegralType()==b.getIntegralType() && this->getValuesPerVertex()==b.getValuesPerVertex());
        }

    GLuint              mGLID;
   // GLuint              mArrayType;
    uint                mBufferSize;
    BUFFER_ELEMENT_TYPE mBufferElementType;

};


template <class T>
class ArrayBuffer_T : public ArrayBuffer_b
{
public:


    ArrayBuffer_T()
    {
        #define SET_BUFFER_ELEMENT(T2, E1) if( std::is_same<T, T2>::value )  { mBufferElementType = E1; /*printf("ArrayBuffer created with Type: %d\n", (int)E1);*/ }

        SET_BUFFER_ELEMENT(float, F1);
        SET_BUFFER_ELEMENT(vec2,  F2);
        SET_BUFFER_ELEMENT(vec3,  F3);
        SET_BUFFER_ELEMENT(vec4,  F4);

        SET_BUFFER_ELEMENT(unsigned int, U1);
        SET_BUFFER_ELEMENT(uvec2,  U2);
        SET_BUFFER_ELEMENT(uvec3,  U3);
        SET_BUFFER_ELEMENT(uvec4,  U4);

        SET_BUFFER_ELEMENT(int,    I1);
        SET_BUFFER_ELEMENT(ivec2,  I2);
        SET_BUFFER_ELEMENT(ivec3,  I3);
        SET_BUFFER_ELEMENT(ivec4,  I4);

        SET_BUFFER_ELEMENT(unsigned char,   uB1);
        SET_BUFFER_ELEMENT(ucol2,           uB2);
        SET_BUFFER_ELEMENT(ucol3,           uB3);
        SET_BUFFER_ELEMENT(ucol4,           uB4);

        #undef SET_BUFFER_ELEMENT

        //printf("ArrayBuffer created with Type: %d\n", (int)E1);
    }


    constexpr BUFFER_ELEMENT_TYPE getBufferType()
    {
        #define CHECK_BUFFER_ELEMENT(T2, E1) if( std::is_same<T, T2>::value )  return(E1);

        CHECK_BUFFER_ELEMENT(float, F1);
        CHECK_BUFFER_ELEMENT(vec2,  F2);
        CHECK_BUFFER_ELEMENT(vec3,  F3);
        CHECK_BUFFER_ELEMENT(vec4,  F4);
        CHECK_BUFFER_ELEMENT(unsigned int, U1);
        CHECK_BUFFER_ELEMENT(uvec2,  U2);
        CHECK_BUFFER_ELEMENT(uvec3,  U3);
        CHECK_BUFFER_ELEMENT(uvec4,  U4);
        CHECK_BUFFER_ELEMENT(int,    I1);
        CHECK_BUFFER_ELEMENT(ivec2,  I2);
        CHECK_BUFFER_ELEMENT(ivec3,  I3);
        CHECK_BUFFER_ELEMENT(ivec4,  I4);

        CHECK_BUFFER_ELEMENT(unsigned char,   uB1);
        CHECK_BUFFER_ELEMENT(ucol2,           uB2);
        CHECK_BUFFER_ELEMENT(ucol3,           uB3);
        CHECK_BUFFER_ELEMENT(ucol4,           uB4);
        #undef CHECK_BUFFER_ELEMENT
    }

    inline T & operator[](int i)
    {
        return mVector[i];
    }

    /**
     * Gets the number of vertices in the buffer.
     */
    virtual uint  getVertexCount() const { return (uint)mVector.size(); };


    /**
     * Gets a pointer to the start of the raw vertex data.
     */
    virtual void* getData()        const { return (void*)mVector.data(); };


    /**
     * Gets the number of bytes in the raw buffer. This is different
     * from getVertexCount.
     */
    virtual uint  getByteSize() const
    {
        return( (uint)(mVector.size() * sizeof(T)) );
    };


    virtual uint size() const
    {
        return (uint)(mVector.size()  );
    };

    /**
     * Gets the number of values per vertex. If representing a vertex in 3d spaces, this value
     * will be 3.
     */
    inline virtual uint getValuesPerVertex() const
    {
        #define CHECK_BUFFER_ELEMENT(T2, E1) if( std::is_same<T, T2>::value )  return(E1)

        CHECK_BUFFER_ELEMENT(float, 1);
        CHECK_BUFFER_ELEMENT(vec2,  2);
        CHECK_BUFFER_ELEMENT(vec3,  3);
        CHECK_BUFFER_ELEMENT(vec4,  4);
        CHECK_BUFFER_ELEMENT(unsigned int, 1);
        CHECK_BUFFER_ELEMENT(uvec2,  2);
        CHECK_BUFFER_ELEMENT(uvec3,  3);
        CHECK_BUFFER_ELEMENT(uvec4,  4);
        CHECK_BUFFER_ELEMENT(int,    1);
        CHECK_BUFFER_ELEMENT(ivec2,  2);
        CHECK_BUFFER_ELEMENT(ivec3,  3);
        CHECK_BUFFER_ELEMENT(ivec4,  4);

        CHECK_BUFFER_ELEMENT(unsigned char,   1);
        CHECK_BUFFER_ELEMENT(ucol2,           2);
        CHECK_BUFFER_ELEMENT(ucol3,           3);
        CHECK_BUFFER_ELEMENT(ucol4,           4);

        #undef CHECK_BUFFER_ELEMENT
    }

    /**
     * Inserts a vertex into the buffer.
     *
     * @param v the vertex to insert.
     */
    void insert(const T & v) { mVector.push_back(v);  };

    /**
     * Clears the buffer data
     */
    void clear() { mVector.clear(); };

    /**
     * Adds an offset to all the vertices in the buffer.
     *
     * @param offsetValue The value to add to every vertex
     */
    void addOffset(const T & offsetValue)
    {
        for(int i=0;i<mVector.size();i++) mVector[i] += offsetValue;
    }

    /**
     * Gets a reference to the vertex at a specific index.
     *
     * @param i the index value.
     */
    T & getVertex(int i) { return mVector[i]; };

    private:
        std::vector<T> mVector;

};


typedef gla::ArrayBuffer_T<vec2>  v2ArrayBuffer;
typedef gla::ArrayBuffer_T<vec3>  v3ArrayBuffer;
typedef gla::ArrayBuffer_T<vec4>  v4ArrayBuffer;

typedef gla::ArrayBuffer_T<uvec2> u2ArrayBuffer;
typedef gla::ArrayBuffer_T<uvec3> u3ArrayBuffer;
typedef gla::ArrayBuffer_T<uvec4> u4ArrayBuffer;

}

#endif // ARRAYBUFFER_H

