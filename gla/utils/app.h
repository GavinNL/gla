#ifndef GLA_UTILS_APP_H
#define GLA_UTILS_APP_H

//#include "gla/utils/window.h"
#include "gla/core/timer.h"
#include <map>
#include <functional>


#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

namespace gla  {
namespace utils {

struct KeyEvent
{
    int key;
    int scancode;
    int action;
    int mods;
};

struct ButtonEvent
{
    int button;
    int action;
    int mods;
    double x;
    double y;
};

struct MouseEvent
{
    double x;
    double y;
    double dx;
    double dy;
};

struct FrameEvent
{
    float  t;  // elapsed time
    float dt;  // time since last frame event
};

struct InputEvent
{
    enum {KEY, MOUSE, BUTTON, FRAME} type;
    union
    {
        KeyEvent    Key;
        ButtonEvent Button;
        MouseEvent  Mouse;

        FrameEvent  Frame;
    };
};


// void _KeyCallback        (GLFWwindow* window, int    key,    int    scancode, int action, int mods);
// void _MouseButtonCallback(GLFWwindow* window, int    button, int    action,   int mods);
// void _MousePosCallback   (GLFWwindow* window, double xpos,   double ypos);

class RootApp
{
    public:
        static bool isKeyDown(   int Key)    { return (bool)KEY[Key];       };
        static bool isButtonDown(int Button) { return (bool)BUTTON[Button]; };


        static void Initialize(int W, int H, const char * title)
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

            //std::map<GLFWwindow*, std::weak_ptr<Window> > Window::WindowMap = std::map<GLFWwindow*, std::weak_ptr<Window> >();

            mTimer = Timer_T<float>();
            KEY = std::map<int,int>();
            BUTTON = std::map<int,int>();
            InputEvents = std::map<std::string, std::function<void(InputEvent&)> >();

            glfwSetKeyCallback          ( mWindow, RootApp::_KeyCallback);
            glfwSetCursorPosCallback    ( mWindow, RootApp::_MousePosCallback);
            glfwSetMouseButtonCallback  ( mWindow, RootApp::_MouseButtonCallback);
        }

        static void terminate()
        {
            glfwDestroyWindow(mWindow);
            glfwTerminate();
        }


        static void _KeyCallback        (GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            InputEvent K;
            K.type = InputEvent::KEY;

            K.Key = {key, scancode, action, mods};

            KEY[key]=action;

            for(auto a : InputEvents) a.second(K);
        }

        static  void _MousePosCallback   (GLFWwindow* window, double xpos, double ypos)
        {
            static double oldX = xpos;
            static double oldY = ypos;

            InputEvent K;

            K.type  = InputEvent::MOUSE;
            K.Mouse = { xpos, ypos, xpos-oldX, ypos-oldY };


            oldX = xpos;
            oldY = ypos;
            for(auto a : InputEvents) a.second(K);
        }

        static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            InputEvent K;
            K.Button = {button, action, mods};
            K.type = InputEvent::BUTTON;
            BUTTON[button]=action;
            for(auto a : InputEvents) a.second(K);
        }

        static bool isRunning()
        {

            {
                InputEvent F;
                F.type = InputEvent::FRAME;
                F.Frame.dt = mTimer.getElapsedTime();
                for(auto a : InputEvents) a.second( F );
                mTimer.reset();
            }

           // glfwSwapBuffers(mWindow);
            glfwPollEvents();

            return !glfwWindowShouldClose(gla::utils::RootApp::mWindow);
        }

        static void AddInputReceiver(std::string Name, std::function<void(InputEvent&)> receiver)
        {
            InputEvents[Name] = receiver;
        }

        static void RemoveInputReceiver(std::string Name)
        {
            InputEvents.erase(Name);
        }

    static Timer_T<float>                                           mTimer;
    static GLFWwindow*                                              mWindow;
    static std::map<int, int>                                       KEY;
    static std::map<int, int>                                       BUTTON;
    static std::map<std::string, std::function<void(InputEvent&)> > InputEvents;

};

/*
class App {

    public:


        void CreateWindow( unsigned int width, unsigned height, const char * title )
        {
            mWindow = gla::utils::Window::create(width, height, title);
        };

        ~App()
        {
            glfwTerminate();
        }

        virtual void init( ) { };

        void start()
        {
            this->init();

            Timer_T <double> T;

            double dt = 0.0;
            double t  = 0.0;

            while( !mWindow->WantsToClose() )
            {
                gla::utils::Window::PollEvents();

                this->render(dt, t);

                mWindow->SwapBuffers();

                dt = T.getElapsedTime();
                t += dt;
                T.reset();
            }


        }

        std::shared_ptr<Window> getWindow() { return mWindow; }

        virtual bool render(double dt, double t){ return false; };
        virtual void clean() {  };


    protected:
        std::shared_ptr<Window> mWindow;

};
*/
};
};

#endif // APP_H

