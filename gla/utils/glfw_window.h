#ifndef GLFW_Window_GLA
#define GLFW_Window_GLA

#include<functional>
#include <memory>
#include <vector>
#include <map>

#include <GLFW/glfw3.h>

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

enum class GLFW_Button
{
      _1         =  GLFW_MOUSE_BUTTON_1     ,
      _2         =  GLFW_MOUSE_BUTTON_2     ,
      _3         =  GLFW_MOUSE_BUTTON_3     ,
      _4         =  GLFW_MOUSE_BUTTON_4     ,
      _5         =  GLFW_MOUSE_BUTTON_5     ,
      _6         =  GLFW_MOUSE_BUTTON_6     ,
      _7         =  GLFW_MOUSE_BUTTON_7     ,
      _8         =  GLFW_MOUSE_BUTTON_8     ,
      LAST      =  GLFW_MOUSE_BUTTON_LAST  ,
      LEFT      =  GLFW_MOUSE_BUTTON_LEFT  ,
      RIGHT     =  GLFW_MOUSE_BUTTON_RIGHT ,
      MIDDLE    =  GLFW_MOUSE_BUTTON_MIDDLE
};

enum class GLFW_Key
{
    /* The unknown key */
    UNKNOWN            =GLFW_KEY_UNKNOWN        ,
    SPACE              =GLFW_KEY_SPACE          ,
    APOSTROPHE         =GLFW_KEY_APOSTROPHE     ,
    COMMA              =GLFW_KEY_COMMA          ,
    MINUS              =GLFW_KEY_MINUS          ,
    PERIOD             =GLFW_KEY_PERIOD         ,
    SLASH              =GLFW_KEY_SLASH          ,
    _0                  =GLFW_KEY_0             ,
    _1                  =GLFW_KEY_1             ,
    _2                  =GLFW_KEY_2             ,
    _3                  =GLFW_KEY_3             ,
    _4                  =GLFW_KEY_4             ,
    _5                  =GLFW_KEY_5             ,
    _6                  =GLFW_KEY_6             ,
    _7                  =GLFW_KEY_7             ,
    _8                  =GLFW_KEY_8             ,
    _9                  =GLFW_KEY_9             ,
    SEMICOLON          =GLFW_KEY_SEMICOLON      ,
    EQUAL              =GLFW_KEY_EQUAL          ,
    A                  =GLFW_KEY_A              ,
    B                  =GLFW_KEY_B              ,
    C                  =GLFW_KEY_C              ,
    D                  =GLFW_KEY_D              ,
    E                  =GLFW_KEY_E              ,
    F                  =GLFW_KEY_F              ,
    G                  =GLFW_KEY_G              ,
    H                  =GLFW_KEY_H              ,
    I                  =GLFW_KEY_I              ,
    J                  =GLFW_KEY_J              ,
    K                  =GLFW_KEY_K              ,
    L                  =GLFW_KEY_L              ,
    M                  =GLFW_KEY_M              ,
    N                  =GLFW_KEY_N              ,
    O                  =GLFW_KEY_O              ,
    P                  =GLFW_KEY_P              ,
    Q                  =GLFW_KEY_Q              ,
    R                  =GLFW_KEY_R              ,
    S                  =GLFW_KEY_S              ,
    T                  =GLFW_KEY_T              ,
    U                  =GLFW_KEY_U              ,
    V                  =GLFW_KEY_V              ,
    W                  =GLFW_KEY_W              ,
    X                  =GLFW_KEY_X              ,
    Y                  =GLFW_KEY_Y              ,
    Z                  =GLFW_KEY_Z              ,
    LEFT_BRACKET       =GLFW_KEY_LEFT_BRACKET   ,
    BACKSLASH          =GLFW_KEY_BACKSLASH      ,
    RIGHT_BRACKET      =GLFW_KEY_RIGHT_BRACKET  ,
    GRAVE_ACCENT       =GLFW_KEY_GRAVE_ACCENT   ,
    WORLD_1            =GLFW_KEY_WORLD_1        ,
    WORLD_2            =GLFW_KEY_WORLD_2        ,
    ESCAPE             =GLFW_KEY_ESCAPE         ,
    ENTER              =GLFW_KEY_ENTER          ,
    TAB                =GLFW_KEY_TAB            ,
    BACKSPACE          =GLFW_KEY_BACKSPACE      ,
    INSERT             =GLFW_KEY_INSERT         ,
    DEL                =GLFW_KEY_DELETE         ,
    RIGHT              =GLFW_KEY_RIGHT          ,
    LEFT               =GLFW_KEY_LEFT           ,
    DOWN               =GLFW_KEY_DOWN           ,
    UP                 =GLFW_KEY_UP             ,
    PAGE_UP            =GLFW_KEY_PAGE_UP        ,
    PAGE_DOWN          =GLFW_KEY_PAGE_DOWN      ,
    HOME               =GLFW_KEY_HOME           ,
    END                =GLFW_KEY_END            ,
    CAPS_LOCK          =GLFW_KEY_CAPS_LOCK      ,
    SCROLL_LOCK        =GLFW_KEY_SCROLL_LOCK    ,
    NUM_LOCK           =GLFW_KEY_NUM_LOCK       ,
    PRINT_SCREEN       =GLFW_KEY_PRINT_SCREEN   ,
    PAUSE              =GLFW_KEY_PAUSE          ,
    F1                 =GLFW_KEY_F1             ,
    F2                 =GLFW_KEY_F2             ,
    F3                 =GLFW_KEY_F3             ,
    F4                 =GLFW_KEY_F4             ,
    F5                 =GLFW_KEY_F5             ,
    F6                 =GLFW_KEY_F6             ,
    F7                 =GLFW_KEY_F7             ,
    F8                 =GLFW_KEY_F8             ,
    F9                 =GLFW_KEY_F9             ,
    F10                =GLFW_KEY_F10            ,
    F11                =GLFW_KEY_F11            ,
    F12                =GLFW_KEY_F12            ,
    F13                =GLFW_KEY_F13            ,
    F14                =GLFW_KEY_F14            ,
    F15                =GLFW_KEY_F15            ,
    F16                =GLFW_KEY_F16            ,
    F17                =GLFW_KEY_F17            ,
    F18                =GLFW_KEY_F18            ,
    F19                =GLFW_KEY_F19            ,
    F20                =GLFW_KEY_F20            ,
    F21                =GLFW_KEY_F21            ,
    F22                =GLFW_KEY_F22            ,
    F23                =GLFW_KEY_F23            ,
    F24                =GLFW_KEY_F24            ,
    F25                =GLFW_KEY_F25            ,
    KP_0               =GLFW_KEY_KP_0           ,
    KP_1               =GLFW_KEY_KP_1           ,
    KP_2               =GLFW_KEY_KP_2           ,
    KP_3               =GLFW_KEY_KP_3           ,
    KP_4               =GLFW_KEY_KP_4           ,
    KP_5               =GLFW_KEY_KP_5           ,
    KP_6               =GLFW_KEY_KP_6           ,
    KP_7               =GLFW_KEY_KP_7           ,
    KP_8               =GLFW_KEY_KP_8           ,
    KP_9               =GLFW_KEY_KP_9           ,
    KP_DECIMAL         =GLFW_KEY_KP_DECIMAL     ,
    KP_DIVIDE          =GLFW_KEY_KP_DIVIDE      ,
    KP_MULTIPLY        =GLFW_KEY_KP_MULTIPLY    ,
    KP_SUBTRACT        =GLFW_KEY_KP_SUBTRACT    ,
    KP_ADD             =GLFW_KEY_KP_ADD         ,
    KP_ENTER           =GLFW_KEY_KP_ENTER       ,
    KP_EQUAL           =GLFW_KEY_KP_EQUAL       ,
    LEFT_SHIFT         =GLFW_KEY_LEFT_SHIFT     ,
    LEFT_CONTROL       =GLFW_KEY_LEFT_CONTROL   ,
    LEFT_ALT           =GLFW_KEY_LEFT_ALT       ,
    LEFT_SUPER         =GLFW_KEY_LEFT_SUPER     ,
    RIGHT_SHIFT        =GLFW_KEY_RIGHT_SHIFT    ,
    RIGHT_CONTROL      =GLFW_KEY_RIGHT_CONTROL  ,
    RIGHT_ALT          =GLFW_KEY_RIGHT_ALT      ,
    RIGHT_SUPER        =GLFW_KEY_RIGHT_SUPER    ,
    MENU               =GLFW_KEY_MENU           ,
    LAST               =GLFW_KEY_LAST
};

class GLFW_Window
{

public:



    //static void __OnWindowPosition(GLFWwindow* window,int x, int y)                    { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnWindowPosition(x,y); }
    //static void __OnWindowSize(GLFWwindow* window,int width, int height)               { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnWindowSize(width, height); }
    //static void __OnFramebufferSize(GLFWwindow* window,int width, int height)          { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnFramebufferSize(width,height); }
    //static void __OnClose(GLFWwindow* window)                                          { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnClose(); }
    //static void __OnRefresh(GLFWwindow* window)                                        { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnRefresh(); }
    //static void __OnFocus(GLFWwindow* window,int focused)                              { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnFocus(focused); }
    //static void __OnIconify(GLFWwindow* window,int iconified)                          { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnIconify(iconified); }
    //static void __OnMouseEnter(GLFWwindow* window,int entered)                         { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnMouseEnter(entered); }
    //static void __OnScroll(GLFWwindow* window,double x, double y)                      { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnScroll(x,y); }
    //static void __OnCharacter(GLFWwindow* window,unsigned int codepoint)               { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnCharacter(codepoint); }
    //static void __OnCharacterMods(GLFWwindow* window,unsigned int codepoint, int mods) { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnCharacterMods(codepoint, mods); }
    //static void __OnDrop(GLFWwindow* window,int count, const char** paths)             { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->OnDrop(count, paths); }

    static void __OnMouseButton(GLFWwindow* window,int button, int action, int mods)   { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->onMouseButton( static_cast<GLFW_Button>(button) ,action); }
    static void __OnMousePosition(GLFWwindow* window,double x, double y)               { static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->onMouseMove(x,y); }
    static void __OnKey(GLFWwindow* window,int key, int scancode, int action, int mods){ static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window))->onKey( static_cast<GLFW_Key>(key) , action); }

    GLFW_Window( int width, int height, const char * title = "")
    {
       m_Window = SetupOpenGLLibrariesAndCreateWindow(width, height, title );
       SetupCallbacks(m_Window);
    }

    ~GLFW_Window()
    {
        glfwDestroyWindow(m_Window);

        if(count()-- == 0)
        {
            glfwTerminate();
        }
    }

    void SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

    static void Poll()
    {
        glfwPollEvents();
    }

    operator bool()
    {
        return !glfwWindowShouldClose(m_Window);
    }


    inline void ShowCursor(bool b)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, b ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    Signal<void     , double  , double> onMouseMove;
    Signal<void     , GLFW_Button     , int>    onMouseButton;
    Signal<void     , GLFW_Key, int>    onKey;



    private:

        static int & count()
        {
            static int c = 0;
            return c;
        }

        GLFWwindow * m_Window;

        GLFWwindow* SetupOpenGLLibrariesAndCreateWindow(int width, int height, const char * title)
        {
            //    glewExperimental = GL_TRUE;
            if( count() == 0)
            {
                if ( !glfwInit() )
                    exit(EXIT_FAILURE);
            }

            count()++;

            auto gMainWindow = glfwCreateWindow(width, height, title, NULL, NULL);

            if (!gMainWindow)
            {
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            glfwMakeContextCurrent(gMainWindow);

            glfwGetFramebufferSize(gMainWindow, &width, &height);

            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            {
                std::cout << "Failed to initialize OpenGL context" << std::endl;
                return NULL;
            }


            return(gMainWindow);

        }

        void SetupCallbacks( GLFWwindow * w)
        {
            glfwSetWindowUserPointer( w , this);

            //glfwSetWindowPosCallback(      w,  GLFW_Window::__OnWindowPosition);
            //glfwSetWindowSizeCallback(     w,  GLFW_Window::__OnWindowSize);
            //glfwSetFramebufferSizeCallback(w,  GLFW_Window::__OnFramebufferSize);
            //glfwSetWindowCloseCallback(    w,  GLFW_Window::__OnClose);
            //glfwSetWindowRefreshCallback(  w,  GLFW_Window::__OnRefresh);
            //glfwSetWindowFocusCallback(    w,  GLFW_Window::__OnFocus);
            //glfwSetWindowIconifyCallback(  w,  GLFW_Window::__OnIconify);
            //glfwSetCursorEnterCallback(    w,  GLFW_Window::__OnMouseEnter);
            //glfwSetScrollCallback(         w,  GLFW_Window::__OnScroll);
            //glfwSetCharCallback(           w,  GLFW_Window::__OnCharacter);
            //glfwSetCharModsCallback(       w,  GLFW_Window::__OnCharacterMods);
            //glfwSetDropCallback(           w,  GLFW_Window::__OnDrop);

            glfwSetMouseButtonCallback(    w,  GLFW_Window::__OnMouseButton);
            glfwSetCursorPosCallback(      w,  GLFW_Window::__OnMousePosition);
            glfwSetKeyCallback(            w,  GLFW_Window::__OnKey);
        }

};


}

#endif
