#ifndef GLA_UTILS_WINDOW_H
#define GLA_UTILS_WINDOW_H

#define GLFW_INCLUDE_GL_3
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <memory>

#include <gla/core/types.h>
#include <gla/utils/event.h>

namespace gla  {
namespace utils {

class  Window;

class Window
{
    public:

        ~Window();

        void Hide();
        void Show();

        void Minimize();
        void Restore();

        void Destroy();


        void SetCursorMode(MOUSE::CursorMode mode);

        void MakeCurrent();
        void SwapBuffers();
        bool WantsToClose();

        bool isMouseButtonPressed(utils::MOUSE::MouseButton b);

        std::map<std::string, std::function<void(const Event&)> > EventsMap;

        static void PollEvents();
        static std::shared_ptr<Window> create(unsigned int width, unsigned int height, const char * title);

        //======================== CALLBACKS ==============================================================
        static void _ErrorCallback(int error, const char* description);

        static void _KeyCallback        (GLFWwindow* window, int key, int scancode, int action, int mods);
        static void _MousePosCallback   (GLFWwindow* window, double xpos, double ypos);
        static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void _WindowMinimizedCallback(GLFWwindow* window, int iconified);
        static void _WindowCloseCallback    (GLFWwindow* window);
        static void _WindowFocusCallback    (GLFWwindow* window, int focus);
        static void _WindowFileDropCallback (GLFWwindow* window, int count, const char** paths);
        static void _WindowTextCallback     (GLFWwindow* window, unsigned int codepoint, int mods);
        //=================================================================================================


        uvec2 size();
        void  setSize(const uvec2 & size);

    protected:
        Window();

        GLFWwindow* mWindow;

        static std::map<GLFWwindow*, std::weak_ptr<Window> > WindowMap;

        double _xMouse;
        double _yMouse;

};



} //namespace
} //glre
#endif // WINDOW_H
