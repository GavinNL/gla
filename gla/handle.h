#ifndef GLA_HANDLE_H
#define GLA_HANDLE_H

#include <gla/types.h>
//#include <unordered_map>
#include <map>
#include <memory>

namespace gla
{




struct DefaultHandler
{
    inline static void Create(unsigned int & handle) {}
    inline static void Release(unsigned int & handle) {}
    inline static void Bind(unsigned int   & handle) {}
    inline static void Unbind(unsigned int & handle) {}
};

template<typename Type,
         class Handler,
         class HandleInfo>
class Handle
{
public:
    using HandleType = Handle<Type, Handler, HandleInfo>;

    using SharedObject = std::shared_ptr<HandleInfo>;


    Handle() : m_ID(0)
    {
        assert (m_ID==0);
    }

    void Create()
    {
        Release();

        Type id;

        Handler::Create(id);

        m_ID = id;
        std::cout << "********CREATED***************" << std::endl;
        std::cout <<  m_ID << std::endl;
        m_Shared = std::shared_ptr<HandleInfo>(  new HandleInfo(),
                                                 [ = ] ( HandleInfo * info)
        {
            delete info;
            std::cout << "Deleting: " << id << std::endl;
            Type i = id;
            Handler::Release(i);
            i = 0;

        });

    }

    void Release()
    {
        m_Shared.reset();
        m_ID = 0;
    }

    inline void Bind()
    {
        Handler::Bind(m_ID);
    }

    inline void Unbind()
    {
        Handler::Unbind(m_ID);
    }

    inline Type GetID() { return m_ID;}

    const HandleInfo & GetInfo()
    {
        assert(m_ID!=0); return *m_Shared;
    }

    /**
     * @brief __GetInfo
     * @return
     *
     * This shouldn't be used by anyone other that a factory
     */
    HandleInfo & __GetInfo()
    {
        assert(m_ID!=0); return *m_Shared;
    }


    public:
        Type m_ID = 0;
        SharedObject m_Shared;


};

};

#endif // HANDLE_H

