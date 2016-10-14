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


#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

namespace gla  {
namespace utils {

class GLFW_App
{
    public:

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
        static void __OnKey(GLFWwindow* window,int key, int scancode, int action, int mods){ static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnKey(key,scancode,action,mods); }
        static void __OnCharacter(GLFWwindow* window,unsigned int codepoint)               { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnCharacter(codepoint); }
        static void __OnCharacterMods(GLFWwindow* window,unsigned int codepoint, int mods) { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnCharacterMods(codepoint, mods); }
        static void __OnDrop(GLFWwindow* window,int count, const char** paths)             { static_cast<GLFW_App*>(glfwGetWindowUserPointer(window))->OnDrop(count, paths); }

        void SetWindowTitle(const char * title)
        {
            glfwSetWindowTitle(mWindow,title);
        }

        GLFW_App(int W=640, int H=480, const char * title = "Window")
        {
            glewExperimental = GL_TRUE;

            if (!glfwInit())
                exit(EXIT_FAILURE);

            mWindow = glfwCreateWindow(W, H, title, NULL, NULL);


            if (!mWindow)
            {
                glfwTerminate();
                exit(EXIT_FAILURE);
            }

            glfwMakeContextCurrent(mWindow);

            int width, height;
            glfwGetFramebufferSize(mWindow, &width, &height);
            GLenum err = glewInit();


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

};


}

}

#endif // APP_H

