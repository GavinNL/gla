#include "glre/utils/window.h"
#include "glre/utils/event.h"
#include <iostream>

namespace glre {
namespace utils {

//int Something::s_nValue = 1;
std::map<GLFWwindow*, std::weak_ptr<Window> > Window::WindowMap = std::map<GLFWwindow*, std::weak_ptr<Window> >();

Window::Window()
{

}

Window::~Window()
{
    Destroy();
}

void Window::Hide()
{
    if(mWindow) glfwShowWindow	(	mWindow	);
}

void Window::Show()
{

    if(mWindow) glfwHideWindow	(	mWindow	);

}

void Window::Minimize()
{
    if(mWindow) glfwIconifyWindow(mWindow);
}

void Window::Restore()
{
    if(mWindow) glfwRestoreWindow(mWindow);
}

void Window::Destroy()
{
    if(mWindow)
    {
        glfwDestroyWindow(mWindow);
        mWindow = 0;
    }

}


inline void Window::MakeCurrent()
{
    glfwMakeContextCurrent(mWindow);
}

inline void Window::SwapBuffers()
{
    glfwSwapBuffers(mWindow);
}

bool Window::WantsToClose()
{
    return glfwWindowShouldClose(mWindow);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

std::shared_ptr<Window> Window::create(unsigned int width, unsigned int height, const char * title)
{
    static bool IsInit = glfwInit();
    glfwSetErrorCallback( Window::_ErrorCallback );


    auto W = std::shared_ptr<glre::utils::Window>( new Window() );

    W->mWindow = glfwCreateWindow(width, height, title, NULL, NULL);

    W->MakeCurrent();

    glfwSwapInterval(1);

    glfwSetKeyCallback          ( W->mWindow, Window::_KeyCallback);
    glfwSetCursorPosCallback    ( W->mWindow, Window::_MousePosCallback);
    glfwSetMouseButtonCallback  ( W->mWindow, Window::_MouseButtonCallback);
    glfwSetWindowCloseCallback  ( W->mWindow, Window::_WindowCloseCallback);
    glfwSetWindowFocusCallback  ( W->mWindow, Window::_WindowFocusCallback);
    glfwSetWindowIconifyCallback( W->mWindow, Window::_WindowMinimizedCallback);

    Window::WindowMap[W->mWindow] = W;
    return W;
}


void Window::_ErrorCallback(int error, const char* description)
{
    fputs(description, stderr);
}

void Window::_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Event E;
    E.type         = KEY;
    E.Key.key      = key;
    E.Key.scancode = scancode;
    E.Key.action   = action;
    E.Key.mods     = mods;

    auto W = Window::WindowMap[window].lock();
    if(W)
    {
        for(auto a : W->EventsMap)
        {
            a.second(E);
        }
    } else {
        Window::WindowMap.erase(window);
    }


   // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   //     glfwSetWindowShouldClose(window, GL_TRUE);
}

void Window::_MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Event E;
    E.type          = MOUSECURSOR;
    E.MouseCursor.x = xpos;
    E.MouseCursor.y = ypos;

    auto W = Window::WindowMap[window].lock();
    if(W)
    {
        for(auto a : W->EventsMap)
        {
            a.second(E);
        }
    } else {
        Window::WindowMap.erase(window);
    }
}

void Window::_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Event E;
    E.type   = MOUSEBUTTON;
    E.MouseButton.button = button;
    E.MouseButton.action = action;
    E.MouseButton.mods   = mods;

    auto W = Window::WindowMap[window].lock();
    if(W)
    {
        for(auto a : W->EventsMap)
        {
            a.second(E);
        }
    } else {
        Window::WindowMap.erase(window);
    }
}

void Window::_WindowMinimizedCallback(GLFWwindow *window, int iconified)
{
    Event E;
    E.type   = WINDOW;
    E.Window.action = iconified ? glre::utils::MINIMIZED : glre::utils::RESTORED;

    auto W = Window::WindowMap[window].lock();
    if(W)
    {
        for(auto a : W->EventsMap)
        {
            a.second(E);
        }
    } else {
        Window::WindowMap.erase(window);
    }
}

void Window::_WindowCloseCallback(GLFWwindow *window)
{
    Event E;
    E.type   = WINDOW;
    E.Window.action = glre::utils::CLOSED;

    auto W = Window::WindowMap[window].lock();
    if(W)
    {
        for(auto a : W->EventsMap)
        {
            a.second(E);
        }
    } else {
        Window::WindowMap.erase(window);
    }
}

void Window::_WindowFocusCallback(GLFWwindow *window, int focus)
{
    Event E;
    E.type   = WINDOW;
    E.Window.action = focus ? glre::utils::FOCUS_GAINED : glre::utils::FOCUS_LOST;

    auto W = Window::WindowMap[window].lock();
    if(W)
    {
        for(auto a : W->EventsMap)
        {
            a.second(E);
        }
    } else {
        Window::WindowMap.erase(window);
    }
}

}
}
