#ifndef EXP_HANDLE
#define EXP_HANDLE

#include <iostream>
#include <memory>
namespace gla { namespace experimental
{


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
    using Handle = BaseHandle<HandleType, CallableCreate, CallableDestroy, SharedDataType>;

    BaseHandle() : m_ID(0), ref_count( new SharedDataType() )
    {
    }

    ~BaseHandle()
    {
        Release();
    }


    HandleType Get() const
    {
        return m_ID;
    }

    void Generate()
    {
        Release();

        static CallableCreate C;
        C(m_ID);
    }

    void Release()
    {
        if( ref_count.use_count() == 1)
        {
            if(m_ID)
            {
                static CallableDestroy D;
                D(m_ID);
            }
        }
        ref_count.reset( new SharedDataType() );
        m_ID = 0;

    }

    SharedDataType * SharedData()
    {
        return ref_count.get();
    }

    operator bool() const
    {
        return m_ID != 0;
    }



    protected:
        HandleType                      m_ID = 0;
        std::shared_ptr<SharedDataType> ref_count;
};







} }


#endif
