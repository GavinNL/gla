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



template<typename HandleType, typename CallableCreate, typename CallableDestroy>
class BaseHandle
{

public:
    using Handle = BaseHandle<HandleType, CallableCreate, CallableDestroy>;

    BaseHandle() : m_ID(0), ref(nullptr)
    {
        ref = new RefCounter();
        ref->inc();
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
        if( ref ) // if already referenced
        {
            Release();
        }

        static CallableCreate C;
        C(m_ID);
        ref = new RefCounter();
        ref->inc();
        std::cout << "ID Generated: " << m_ID << "     use count: " << ref->refcount() << std::endl;
    }

    void Release()
    {
        if( ref)
        {
            if( ref->dec() == 0)
            {
                if( m_ID )
                {
                    static CallableDestroy D;
                    D(m_ID);

                }
                delete ref;
            }
        }

        m_ID = 0;
        ref  = nullptr;
    }

    operator bool() const
    {
        return m_ID != 0;
    }

    Handle & operator = (const Handle & sp)
    {
        if( this != &sp )
        {
            Release();

            m_ID = sp.m_ID;
            ref  = sp.ref;
            ref->inc();
        }
    }

    protected:
        HandleType m_ID = 0;
        RefCounter * ref;
};

template<typename HandleType, typename CallableCreate, typename CallableDestroy>
class BaseHandle_old
{
    public:
        using Handle = BaseHandle<HandleType, CallableCreate, CallableDestroy>;

        BaseHandle_old() : ref( new RefCounter() ) , m_ID(0)
        {
        }

        ~BaseHandle_old()
        {
            Release();
        }




        //===============================================
        // Creates a new buffer. If this buffer already
        // contains some data, it will be released.
        // if it is the only one that holds data, the data will be
        // destroyed.
        void Generate()
        {
            Release();

            static CallableCreate C;
            C(m_ID);

            ref = new RefCounter();
            ref->inc();
        }

        // Releases the buffer, returns true if the object has been
        // completely deleted (ie: No other objects are referenced to it)
        bool Release()
        {
            if( !(*this) )
            {
                m_ID = 0;
                return true;
            }

            auto ret = false;

            auto r = ref->dec();
            std::cout << "   " << m_ID << " ref count: " << ref->refcount() << std::endl;
            if( r==0 ) // destroy the GL object and delete the reference counter
            {                     // if it this is the last reference

                static CallableDestroy D;
                D(m_ID);

                ret = true;
                delete ref;
            }

            ref = nullptr;

            m_ID = 0;
            return ret;
        }

        const HandleType & Get() const
        {
            return m_ID;
        }

        operator bool() const
        {
            return ref != nullptr;
        }

        std::size_t GetUseCount() const
        {
            if( ref )
            {
                return ref->refcount();
            }
            return 0;
        }

        BaseHandle<HandleType, CallableCreate,CallableDestroy> & operator=(const BaseHandle<HandleType, CallableCreate,CallableDestroy> & h)
        {
            std::cout << "Copy operator" << std::endl;

            if(*this)
                Release();

            delete ref;

            m_ID = h.m_ID;
            ref  = h.ref;

            ref->inc();
            std::cout << "   " << m_ID << " ref count: " << ref->refcount() << std::endl;
            return *this;
        }

        //================================================

    private:

        HandleType  m_ID = 0;
        RefCounter * ref;



};






} }


#endif
