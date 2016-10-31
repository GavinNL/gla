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

#ifndef GLA_UTILS_APP_H
#define GLA_UTILS_APP_H

#include <gla/core/timer.h>
#include <map>
#include <functional>


//#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

namespace gla  {
namespace utils {

class GLFW_App
{
    public:
    struct Key
    {

        enum Value
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
            DELETE             =GLFW_KEY_DELETE         ,
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
        Value v;
    };

        virtual void OnWindowPosition(int x, int y){}
        virtual void OnWindowSize(int width, int height){}
        virtual void OnFramebufferSize(int width, int height){}
        virtual void OnClose(){}
        virtual void OnRefresh(){}
        virtual void OnFocus(int focused){}
        virtual void OnIconify(int iconified){}
        virtual void OnMouseButton(int button, int action, int mods){}
        virtual void OnMousePosition(double x, double y){}
        virtual void OnMouseEnter(int entered){}
        virtual void OnScroll(double x, double y){}

        virtual void OnKey(int key, int scancode, int action, int mods){}

        virtual void OnCharacter(unsigned int codepoint){}
        virtual void OnCharacterMods(unsigned int codepoint, int mods){}
        virtual void OnDrop(int count, const char** paths){}

        virtual void onStart() {}
        virtual void onFrame(double dt){}

        static void __OnWindowPosition(GLFWwindow* window,int x, int y)                    { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnWindowPosition(x,y); }
        static void __OnWindowSize(GLFWwindow* window,int width, int height)               { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnWindowSize(width, height); }
        static void __OnFramebufferSize(GLFWwindow* window,int width, int height)          { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnFramebufferSize(width,height); }
        static void __OnClose(GLFWwindow* window)                                          { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnClose(); }
        static void __OnRefresh(GLFWwindow* window)                                        { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnRefresh(); }
        static void __OnFocus(GLFWwindow* window,int focused)                              { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnFocus(focused); }
        static void __OnIconify(GLFWwindow* window,int iconified)                          { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnIconify(iconified); }
        static void __OnMouseButton(GLFWwindow* window,int button, int action, int mods)   { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnMouseButton(button,action,mods); }
        static void __OnMousePosition(GLFWwindow* window,double x, double y)               { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnMousePosition(x,y); }
        static void __OnMouseEnter(GLFWwindow* window,int entered)                         { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnMouseEnter(entered); }
        static void __OnScroll(GLFWwindow* window,double x, double y)                      { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnScroll(x,y); }
        static void __OnKey(GLFWwindow* window,int key, int scancode, int action, int mods){ static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnKey( key,scancode,action,mods); }
        static void __OnCharacter(GLFWwindow* window,unsigned int codepoint)               { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnCharacter(codepoint); }
        static void __OnCharacterMods(GLFWwindow* window,unsigned int codepoint, int mods) { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnCharacterMods(codepoint, mods); }
        static void __OnDrop(GLFWwindow* window,int count, const char** paths)             { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnDrop(count, paths); }

        void SetWindowTitle(const char * title)
        {
            glfwSetWindowTitle(mWindow,title);
        }


        void run()
        {
            double t;
            m_Timer.reset();
            m_Quit = false;

            while( !glfwWindowShouldClose(mWindow) )
            {
                double dt = m_Timer.getElapsedTime(); m_Timer.reset();
                t += dt;

                this->onFrame(dt);


                PollEvents();
                SwapBuffers();
            }
        }

        GLFW_App(int W=640, int H=480, const char * title = "Window")
        {

            mWindow = SetupOpenGLLibrariesAndCreateWindow(W, H, title);

             //glfwSetKeyCallback          ( mWindow, GLFW_App::_KeyCallback);
            //glfwSetCursorPosCallback    ( mWindow, GLFW_App::_MousePosCallback);
            //glfwSetMouseButtonCallback  ( mWindow, GLFW_App::_MouseButtonCallback);

            glfwSetWindowUserPointer( mWindow , this);

            glfwSetWindowPosCallback(      mWindow,  GLFW_App::__OnWindowPosition);
            glfwSetWindowSizeCallback(     mWindow,  GLFW_App::__OnWindowSize);
            glfwSetFramebufferSizeCallback(mWindow,  GLFW_App::__OnFramebufferSize);
            glfwSetWindowCloseCallback(    mWindow,  GLFW_App::__OnClose);
            glfwSetWindowRefreshCallback(  mWindow,  GLFW_App::__OnRefresh);
            glfwSetWindowFocusCallback(    mWindow,  GLFW_App::__OnFocus);
            glfwSetWindowIconifyCallback(  mWindow,  GLFW_App::__OnIconify);
            glfwSetMouseButtonCallback(    mWindow,  GLFW_App::__OnMouseButton);
            glfwSetCursorPosCallback(      mWindow,  GLFW_App::__OnMousePosition);
            glfwSetCursorEnterCallback(    mWindow,  GLFW_App::__OnMouseEnter);
            glfwSetScrollCallback(         mWindow,  GLFW_App::__OnScroll);
            glfwSetKeyCallback(            mWindow,  GLFW_App::__OnKey);
            glfwSetCharCallback(           mWindow,  GLFW_App::__OnCharacter);
            glfwSetCharModsCallback(       mWindow,  GLFW_App::__OnCharacterMods);
            glfwSetDropCallback(           mWindow,  GLFW_App::__OnDrop);
        }

        GLFWwindow* SetupOpenGLLibrariesAndCreateWindow(int w, int h, const char * window_title)
        {
            if (!glfwInit())
                exit(EXIT_FAILURE);

            auto gMainWindow = glfwCreateWindow(w, h, window_title, NULL, NULL);

            if (!gMainWindow)
            {
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            glfwMakeContextCurrent(gMainWindow);


            glfwGetFramebufferSize(gMainWindow, &_width, &_height);
            //    GLenum err = glewInit();
            //std::cout << "Window Created: " << Width() << "x" << Height() << std::endl;

            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            {
                std::cout << "Failed to initialize OpenGL context" << std::endl;
                return NULL;
            }


            return(gMainWindow);

        }

        void Quit()
        {
            m_Quit = true;
        }

        int Width() const { return _width;}
        int Height() const { return _height;}
        inline void SwapBuffers() { glfwSwapBuffers(mWindow); }
        inline void PollEvents() { glfwPollEvents(); }

        inline void ShowCursor(bool b)
        {
            glfwSetInputMode(mWindow, GLFW_CURSOR, b ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        ~GLFW_App()
        {
            glfwDestroyWindow(mWindow);
        }

        static void terminate()
        {
           // glfwDestroyWindow(mWindow);
            glfwTerminate();
        }




protected:
            GLFWwindow * mWindow;
            gla::Timer_T<double> m_Timer;
            bool m_Quit = false;
            int _width;
            int _height;

};


}

}

#endif // APP_H

