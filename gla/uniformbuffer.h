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
    int  Size;
};

enum class GL_UniformBufferInfo
{
    MAX_UNIFORM_BUFFER_BINDINGS = GL_MAX_UNIFORM_BUFFER_BINDINGS,
    MAX_UNIFORM_BLOCK_SIZE      = GL_MAX_UNIFORM_BLOCK_SIZE,
    MAX_COMBINED_UNIFORM_BLOCKS = GL_MAX_COMBINED_UNIFORM_BLOCKS
};

class GPUUniformBuffer_new

{
    using HandleType = Handle<GLuint, UniformBufferHandler, UniformBufferInfo>;


    public:
    HandleType m_Handle;


    inline void Bind()     { m_Handle.Bind();    }
    inline void Unbind()   { m_Handle.Unbind();  }
    inline void Release()  { m_Handle.Release(); }


    void AllocateMemory(std::size_t bytes)
    {
        if( m_Handle.GetID() )
            m_Handle.Release();

        unsigned char temp[bytes];

        m_Handle.Create();
        m_Handle.Bind();


        glBufferData(GL_UNIFORM_BUFFER, bytes, &temp[0], GL_DYNAMIC_COPY);

        Unbind();

        m_Handle.__GetInfo().Size = bytes;


        if( m_Handle.GetID() == -1)
        {
            throw std::runtime_error("Uniform Buffer not created\n");
        } else {
            std::cout << "Uniform Buffer Created. Size: " << bytes << std::endl;
        }
    }

    template<typename T>
    void AllocateMemory()
    {
        AllocateMemory( sizeof(T) );
    }

    template<typename T>
    void AllocateMemory(const T & ProperlyAlignedStruct)
    {
        AllocateMemory( sizeof(T) );
        CopyData<T>(ProperlyAlignedStruct);
    }

    template<class T>
    void CopyData(const T & ProperlyAlignedStruct)
    {
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, m_Handle.GetInfo().Size, &ProperlyAlignedStruct);
    }

    inline void SetBindPoint(GLuint BindPoint)
    {
        Bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, BindPoint, m_Handle.GetID() );
    }

    static GLuint GL_Info( GL_UniformBufferInfo I)
    {
        switch(I)
        {
            case GL_UniformBufferInfo::MAX_UNIFORM_BUFFER_BINDINGS:
            {
                static GLint x = 0;
                if( x ) return x;
                glGetIntegerv (GL_MAX_UNIFORM_BUFFER_BINDINGS, &x);
                return x;
            }

            case GL_UniformBufferInfo::MAX_UNIFORM_BLOCK_SIZE:
            {
                static GLint x = 0;
                if( x ) return x;
                glGetIntegerv (GL_MAX_UNIFORM_BLOCK_SIZE, &x);
                return x;
            }
            case GL_UniformBufferInfo::MAX_COMBINED_UNIFORM_BLOCKS:
            {
                static GLint x = 0;
                if( x ) return x;
                glGetIntegerv (GL_MAX_COMBINED_UNIFORM_BLOCKS, &x);
                return x;
            }
        }
    }
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
        GLA_DOUT  << "Uniform Bufer created" << std::endl;
        GLA_DOUT  << "  size: " << size << std::endl;
        GLA_DOUT  << "   ubo: " << mUBO << std::endl;
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

