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

#ifndef EXP_HANDLE
#define EXP_HANDLE

#include <iostream>
#include <memory>
namespace gla {


class RefCounter
{
    int count=0;

    public:

        RefCounter() : count(0) {}

        int refcount() const { return count; }

        void inc()
        {
            count++;
        }

        int dec()
        {
            return --count;
        }

};

enum class Unique_Handle_Name
{
    Buffer,
    Vertex_Buffer,
    Element_Buffer,
    Vertex_Array,
    Sampler2D,
    Sampler2DArray,
};



/**
 * @brief The BaseHandle class
 * The BaseHandle class is a wrapper around a generic openGL handle type.
 *
 * It provides reference counting which auto releases the object from the GPU.
 *
 * This class should be inherited from
 *
 * Templates Arguments:
 *
 *   HandleType     - the base data type that the OpenGL handle is stored as, usually GLuint, or GLint
 *   CallableCreate - A callable class with the operator()(HandleType & h) implemented which creates
 *                    the object on the GPU and stores the handle in h
 *   CallableDestroy- A callable class with the operator()(HandleType & h) implemented which destroys
 *                    handle, h, and resets h to it's default states
 *
 *   SharedData     - a struct which contains data about the handle that is not used regularly, such as
 *                    size of buffers, etc.
 */
template<typename HandleType, typename CallableCreate, typename CallableDestroy, typename SharedDataType=int>
class BaseHandle
{

public:
    using GL_id_type       = HandleType;
    using Handle           = BaseHandle<HandleType, CallableCreate, CallableDestroy, SharedDataType>;
    using SharedHandle     = std::shared_ptr< std::pair<HandleType, SharedDataType> >;

    struct Deleter
    {
        void operator()( std::pair<HandleType, SharedDataType> * h ) const
        {
            static CallableDestroy D;
            if(h->first) D( h->first );

            delete h;
        }
    };

    BaseHandle() : m_ID( new std::pair<HandleType, SharedDataType>(),  Deleter() )//, ref_count( new SharedDataType() )
    {
        m_ID->first = 0;
    }

    BaseHandle(const BaseHandle & B) : m_ID( B.m_ID )//, ref_count( B.ref_count)
    {

    }

    BaseHandle(BaseHandle && B)
    {
        m_ID = std::move( B.m_ID );
    }

    BaseHandle & operator=(const BaseHandle & B)
    {
        if( this != &B)
        m_ID = B.m_ID;
        return *this;
    }

    BaseHandle & operator=( BaseHandle && B)
    {
        if( this != &B)
        m_ID = std::move(B.m_ID);
        return *this;
    }

    ~BaseHandle()
    {
    }

    /**
     * @brief Get - returns the openGL handle type
     * @return The OpenGL handle type (GLuint)
     */
    HandleType Get() const
    {
        return m_ID->first;
    }

    /**
     * @brief Generate
     * Generates the OpenGL handle. This will release the previous handle if it exists. Previous handles
     * will not be deallocated unless there are no more references to it.
     */
    void Generate()
    {
        Release();

        static CallableCreate C;
        C(m_ID->first);
    }

    /**
     * @brief Regenerate
     * Regnerates the OpenGL handle. This will destroy the data and regenerate it. All references
     * to the old handle will not point to the new handle.
     */
    void Regenerate()
    {
        static CallableDestroy D;
        static CallableCreate C;

        if(m_ID->first) D( m_ID->first );
        C( m_ID->first);
    }

    void Release()
    {
        m_ID.reset( new std::pair<HandleType, SharedDataType>() , Deleter() );
        m_ID->first = 0;
    }

    void ForceDelete()
    {
        static CallableDestroy D;

        if(m_ID->first) D( m_ID->first );

        m_ID->second = SharedDataType();
    }

    SharedDataType & SharedData() const
    {
        return m_ID->second;//.get();
    }

    SharedDataType & SharedData()
    {
        return m_ID->second;//.get();
    }

    const HandleType & GetHandle() const
    {
        return m_ID->first;
    }

    HandleType & GetHandle()
    {
        return m_ID->first;
    }

    operator bool() const
    {
        return m_ID->first != 0;
    }

    long Use_Count() const {
        return m_ID.use_count();
    }

    SharedHandle GetSharedHandle() const
    {
        return m_ID;
    }

    protected:
        std::shared_ptr<
        std::pair<HandleType, SharedDataType>
        > m_ID;
};







}


#endif
