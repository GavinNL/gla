#ifndef EXP_HANDLE
#define EXP_HANDLE

#include <iostream>

namespace gla { namespace experimental
{


class RefCounter
{
    std::size_t count=0;

    public:
        std::size_t refcount() const { return count; }

        std::size_t inc()
        {
            return ++count;
        }
        std::size_t dec()
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
        //using Handle = gla::experimental::Handle_t< Unique_Handle_Name::Buffer, GLuint>;

        BaseHandle()
        {
        }

        ~BaseHandle()
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
            if(!(*this)) return true;

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
        RefCounter * ref = nullptr;



};






} }


#endif
