#ifndef GLA_UNIFORMBUFFER_H
#define GLA_UNIFORMBUFFER_H

#include <gla/types.h>
#include <iostream>
#include <gla/handle.h>

namespace gla
{


struct UniformBufferHandler
{
    inline static void Create  (GLuint & h) { glGenBuffers(1, &h); }
    inline static void Release (GLuint & h) { glDeleteBuffers(1, &h); }
    inline static void Bind    (GLuint & h) { glBindBuffer(GL_UNIFORM_BUFFER, h); }
    inline static void Unbind  (GLuint & h) { glBindBuffer(GL_UNIFORM_BUFFER, 0);  }
};

struct UniformBufferInfo
{
    int               Size;
};


class GPUUniformBuffer_new : gla::Handle<GLuint, UniformBufferHandler, UniformBufferInfo >

{

};


class GPUUniformBuffer
{
public:


    GPUUniformBuffer(unsigned int bytes=0)
    {
        if(bytes) Create(bytes);
    }


    /**
     * @brief Create
     * @param bytes - number of bytes to allocate on the GPU
     *
     */
    void Create(int bytes)
    {
        clear();

        unsigned char temp[bytes];
        glGenBuffers(1, &mUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, mUBO);

        glBufferData(GL_UNIFORM_BUFFER, bytes, &temp[0], GL_DYNAMIC_COPY);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        size = bytes;
        std::cout << "Uniform Bufer created" << std::endl;
        std::cout << "  size: " << size << std::endl;
        std::cout << "   ubo: " << mUBO << std::endl;
        if( mUBO == -1)
        {
            throw std::runtime_error("Uniform Buffer not created\n");
        } else {

        }



    }

    template<class T>
    void CopyData(const T & ProperlyAlignedStruct)
    {
        bind();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, size, &ProperlyAlignedStruct);
    }

    /**
     * @brief clear
     * Clears the memory on the GPU
     */
    void clear()
    {
        if(mUBO)
        {
            glDeleteBuffers(1, &mUBO);
            mUBO = 0;
        }
    }

    /**
     * @brief bind Binds the uniform buffer so that it can be modified. You probably don't need to call this yourself
     *
     */
    inline void bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
    }

    /**
     * @brief BindBase
     * @param BindPoint - The index in the GPU to bind the buffer to. It must be
     *                    between 0 and Get_MAX_UNIFORM_BUFFER_BINDINGS()
     */
    inline void BindBase(GLuint BindPoint)
    {
        bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, BindPoint, mUBO);
    }

    static GLuint Get_MAX_UNIFORM_BUFFER_BINDINGS()
    {
        static GLint x = 0;
        if( x ) return x;
        glGetIntegerv (GL_MAX_UNIFORM_BUFFER_BINDINGS, &x);
        return x;
    }

    static GLuint Get_MAX_UNIFORM_BLOCK_SIZE()
    {
        static GLint x = 0;
        if( x ) return x;
        glGetIntegerv (GL_MAX_UNIFORM_BLOCK_SIZE, &x);
        return x;
    }

    static GLuint Get_MAX_COMBINED_UNIFORM_BLOCKS()
    {
        static GLint x = 0;
        if( x ) return x;
        glGetIntegerv (GL_MAX_COMBINED_UNIFORM_BLOCKS, &x);
        return x;
    }


    GLuint       mUBO=0;
    unsigned int size=0;
};



}

#endif // UNIFORMBUFFER_H

