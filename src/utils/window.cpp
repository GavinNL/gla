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


//=====
void Window::SetCursorMode(MOUSE::CursorMode mode)
{
    glfwSetInputMode(mWindow, GLFW_CURSOR, mode);
    //glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}


//=====

void Window::MakeCurrent()
{
    glfwMakeContextCurrent(mWindow);

}

void Window::SwapBuffers()
{
    glfwSwapBuffers(mWindow);
}

bool Window::WantsToClose()
{
    return glfwWindowShouldClose(mWindow);
}

bool Window::isMouseButtonPressed(MOUSE::MouseButton b)
{
    return glfwGetMouseButton(mWindow, b );
}

void Window::PollEvents()
{
    glfwPollEvents();
}

std::shared_ptr<Window> Window::create(unsigned int width, unsigned int height, const char * title)
{
//    static bool IsInit = false;

//    if(!IsInit)
//    {
//        glfwInit();
//        IsInit = true;
//        glfwI
//        glewExperimental = GL_TRUE;
//        GLenum err = glewInit();
//    }

    glfwSetErrorCallback( Window::_ErrorCallback );

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto W = std::shared_ptr<glre::utils::Window>( new Window() );

    W->mWindow = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!W->mWindow)
    {
      fprintf (stderr, "ERROR: could not open window with GLFW3\n");
      glfwTerminate();
      return 0;
    }

    std::cout << "Major: " << glfwGetWindowAttrib(W->mWindow,GLFW_CONTEXT_VERSION_MAJOR) << std::endl;
    std::cout << "Minor: " << glfwGetWindowAttrib(W->mWindow,GLFW_CONTEXT_VERSION_MINOR) << std::endl;
    W->MakeCurrent();

    glfwSwapInterval(1);

    glfwSetKeyCallback          ( W->mWindow, Window::_KeyCallback);
    glfwSetCursorPosCallback    ( W->mWindow, Window::_MousePosCallback);
    glfwSetMouseButtonCallback  ( W->mWindow, Window::_MouseButtonCallback);
    glfwSetWindowCloseCallback  ( W->mWindow, Window::_WindowCloseCallback);
    glfwSetWindowFocusCallback  ( W->mWindow, Window::_WindowFocusCallback);
    glfwSetWindowIconifyCallback( W->mWindow, Window::_WindowMinimizedCallback);
    glfwSetDropCallback         ( W->mWindow, Window::_WindowFileDropCallback);
    glfwSetCharModsCallback     ( W->mWindow, Window::_WindowTextCallback);
    Window::WindowMap[W->mWindow] = W;

    glewExperimental = GL_TRUE;
    glewInit();
    return W;
}


void Window::_ErrorCallback(int error, const char* description)
{
    std::cout << "Error: " << description << std::endl;
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
        E.MouseCursor.dx = xpos - W->_xMouse;
        E.MouseCursor.dy = ypos - W->_yMouse;

        for(auto a : W->EventsMap)
        {
            a.second(E);
        }

        W->_xMouse = xpos;
        W->_yMouse = ypos;

    } else {
        Window::WindowMap.erase(window);
    }

}

void Window::_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Event E;
    E.type   = MOUSEBUTTON;
    E.MouseButton.button = static_cast<MOUSE::MouseButton>(button);
    E.MouseButton.action = action;
    E.MouseButton.mods   = mods;
    glfwGetCursorPos(window, &E.MouseButton.x, &E.MouseButton.y);

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

void Window::_WindowFileDropCallback(GLFWwindow *window, int count, const char **paths)
{
    Event E;
    E.type   = FILEDROP;
    E.FileDrop.count = count;
    E.FileDrop.files = paths;

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

void Window::_WindowTextCallback(GLFWwindow *window, unsigned int codepoint, int mods)
{
    Event E;
    E.type           = TEXT;
    E.Text.codepoint = codepoint;
    E.Text.mods      = mods;

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
