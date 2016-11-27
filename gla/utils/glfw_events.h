#ifndef GLFW_EVENTS_GLA
#define GLFW_EVENTS_GLA

#include<functional>
#include <memory>
#include <vector>
#include <map>

#include <GLFW/glfw3native.h>

namespace gla {

template<typename return_type, typename ..._Types>
class Signal
{
    using function_t  = std::function< return_type(_Types...) >;
    using SignalRefId = unsigned long long;
    using container_t = std::map< SignalRefId, function_t >;

    class SignalRef
    {
    public:
        SignalRef( std::shared_ptr<container_t> d, SignalRefId id)  : m_id(id) , m_container(d)
        {

        }

        ~SignalRef()
        {
            if( auto mc = m_container.lock() )
            {
                mc->erase( m_id );
            }
        }

        SignalRefId m_id;
        std::weak_ptr<container_t> m_container;

        friend class Signal<return_type, _Types...>;
    };

    public:
        Signal() : m_slots(  new container_t() )
        {

        }

        void operator()( _Types&&... __args )
        {
            for(auto & f : *m_slots)
                f.second( std::forward<_Types>(__args)...) ;
        }

        void operator()( const _Types&... __args )
        {
            for(auto & f : *m_slots)
                f.second( std::forward<const _Types>(__args)...) ;
        }

        void Disconnect( SignalRef & R )
        {
            m_slots->erase( R.m_id );
            R.m_container.reset();
        }

        SignalRef Connect( function_t f )
        {
            static SignalRefId id=0;
            (*m_slots)[ id ] = f;
            return SignalRef( m_slots, id++ );
        }

        void operator = ( function_t f )
        {
            auto S = Connect(f);
            S.m_container.reset();
        }

        std::shared_ptr< container_t  > m_slots;
};




class glfw_events
{

public:
    //static void __OnWindowPosition(GLFWwindow* window,int x, int y)                    { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnWindowPosition(x,y); }
    //static void __OnWindowSize(GLFWwindow* window,int width, int height)               { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnWindowSize(width, height); }
    //static void __OnFramebufferSize(GLFWwindow* window,int width, int height)          { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnFramebufferSize(width,height); }
    //static void __OnClose(GLFWwindow* window)                                          { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnClose(); }
    //static void __OnRefresh(GLFWwindow* window)                                        { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnRefresh(); }
    //static void __OnFocus(GLFWwindow* window,int focused)                              { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnFocus(focused); }
    //static void __OnIconify(GLFWwindow* window,int iconified)                          { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnIconify(iconified); }
    //static void __OnMouseEnter(GLFWwindow* window,int entered)                         { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnMouseEnter(entered); }
    //static void __OnScroll(GLFWwindow* window,double x, double y)                      { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnScroll(x,y); }
    //static void __OnCharacter(GLFWwindow* window,unsigned int codepoint)               { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnCharacter(codepoint); }
    //static void __OnCharacterMods(GLFWwindow* window,unsigned int codepoint, int mods) { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnCharacterMods(codepoint, mods); }
    //static void __OnDrop(GLFWwindow* window,int count, const char** paths)             { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->OnDrop(count, paths); }

    static void __OnMouseButton(GLFWwindow* window,int button, int action, int mods)   { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->onMouseButton(button,action); }
    static void __OnMousePosition(GLFWwindow* window,double x, double y)               { static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->onMouseMove(x,y); }
    static void __OnKey(GLFWwindow* window,int key, int scancode, int action, int mods){ static_cast<glfw_events*>(glfwGetWindowUserPointer(window))->onKey( key,action); }

    glfw_events( GLFWwindow * w)
    {
        glfwSetWindowUserPointer( w , this);

        //glfwSetWindowPosCallback(      w,  glfw_events::__OnWindowPosition);
        //glfwSetWindowSizeCallback(     w,  glfw_events::__OnWindowSize);
        //glfwSetFramebufferSizeCallback(w,  glfw_events::__OnFramebufferSize);
        //glfwSetWindowCloseCallback(    w,  glfw_events::__OnClose);
        //glfwSetWindowRefreshCallback(  w,  glfw_events::__OnRefresh);
        //glfwSetWindowFocusCallback(    w,  glfw_events::__OnFocus);
        //glfwSetWindowIconifyCallback(  w,  glfw_events::__OnIconify);
        //glfwSetCursorEnterCallback(    w,  glfw_events::__OnMouseEnter);
        //glfwSetScrollCallback(         w,  glfw_events::__OnScroll);
        //glfwSetCharCallback(           w,  glfw_events::__OnCharacter);
        //glfwSetCharModsCallback(       w,  glfw_events::__OnCharacterMods);
        //glfwSetDropCallback(           w,  glfw_events::__OnDrop);

        glfwSetMouseButtonCallback(    w,  glfw_events::__OnMouseButton);
        glfwSetCursorPosCallback(      w,  glfw_events::__OnMousePosition);
        glfwSetKeyCallback(            w,  glfw_events::__OnKey);
    }

    Signal<void , double, double> onMouseMove;
    Signal<void , int   , int>    onMouseButton;
    Signal<void , int   , int>    onKey;


};


}

#endif
