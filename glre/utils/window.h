#ifndef GLRE_UTILS_WINDOW_H
#define GLRE_UTILS_WINDOW_H

#include <GLFW/glfw3.h>
#include <map>
#include <memory>

#include <glre/utils/event.h>

namespace glre  {
namespace utils {

class  Window;

class Window
{
    public:

        typedef enum
        {
            CURSOR_NORMAL        = GLFW_CURSOR_NORMAL,
            CURSOR_HIDDEN        = GLFW_CURSOR_HIDDEN,
            CURSOR_DISABLED      = GLFW_CURSOR_DISABLED
        } CursorMode;

        ~Window();

        void Hide();
        void Show();

        void Minimize();
        void Restore();

        void Destroy();


        void SetCursorMode(CursorMode mode);

        void MakeCurrent();
        void SwapBuffers();
        bool WantsToClose();

        std::map<std::string, std::function<void(const Event&)> > EventsMap;

        static void PollEvents();
        static std::shared_ptr<Window> create(unsigned int width, unsigned int height, const char * title);

        static void _ErrorCallback(int error, const char* description);

        static void _KeyCallback        (GLFWwindow* window, int key, int scancode, int action, int mods);
        static void _MousePosCallback   (GLFWwindow* window, double xpos, double ypos);
        static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void _WindowMinimizedCallback(GLFWwindow* window, int iconified);
        static void _WindowCloseCallback(GLFWwindow* window);
        static void _WindowFocusCallback(GLFWwindow* window, int focus);
        static void _WindowFileDropCallback(GLFWwindow* window, int count, const char** paths);
        static void _WindowTextCallback(GLFWwindow* window, unsigned int codepoint, int mods);

    private:
        Window();

        GLFWwindow* mWindow;


        static std::map<GLFWwindow*, std::weak_ptr<Window> > WindowMap;

};



} //namespace
} //glre
#endif // WINDOW_H
