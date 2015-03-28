#ifndef GLRE_UTILS_EVENT_H
#define GLRE_UTILS_EVENT_H

#include <GLFW/glfw3.h>

namespace gla {
namespace utils {


struct MOUSE
{
    typedef enum
    {
        LEFT_MOUSE_BUTTON    = GLFW_MOUSE_BUTTON_LEFT,
        MIDDLE_MOUSE_BUTTON  = GLFW_MOUSE_BUTTON_MIDDLE,
        RIGHT_MOUSE_BUTTON   = GLFW_MOUSE_BUTTON_RIGHT
    } MouseButton;

    typedef enum
    {
        CURSOR_NORMAL        = GLFW_CURSOR_NORMAL,
        CURSOR_HIDDEN        = GLFW_CURSOR_HIDDEN,
        CURSOR_DISABLED      = GLFW_CURSOR_DISABLED
    } CursorMode;
};


typedef enum
{
    MINIMIZED,
    RESTORED,
    FOCUS_GAINED,
    FOCUS_LOST,
    CLOSED
} WindowAction;


typedef enum
{
    TEXT,
    WINDOW,
    MOUSECURSOR,
    MOUSEBUTTON,
    KEY,
    FILEDROP
} EventType;

struct TextEvent
{
    unsigned int codepoint;
    int mods;
};

struct WindowEvent
{
    WindowAction action;
};

struct MouseCursorEvent
{
    MOUSE::MouseButton button;
    double x;
    double y;
    double dx;
    double dy;
};

struct MouseButtonEvent
{
    int button;
    int action;
    int mods;
    double x;
    double y;
};

struct KeyEvent
{
    int key;
    int scancode;
    int action;
    int mods;
};

struct FileDropEvent{
    int   count;
    const char **files;
};

struct Event
{
    EventType       type;
    union
    {
        WindowEvent      Window;
        KeyEvent         Key;
        TextEvent        Text;
        MouseCursorEvent MouseCursor;
        MouseButtonEvent MouseButton;
        FileDropEvent    FileDrop;
    };
};

} //utils
} //glre

#endif // EVENTS_H



