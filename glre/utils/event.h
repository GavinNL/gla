#ifndef GLRE_UTILS_EVENT_H
#define GLRE_UTILS_EVENT_H

namespace glre {
namespace utils {


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
    KEY
} EventType;

struct TextEvent
{

};

struct WindowEvent
{
    WindowAction action;
};

struct MouseCursorEvent
{
    double x;
    double y;
};

struct MouseButtonEvent
{
    int button;
    int action;
    int mods;
};

struct KeyEvent
{
    int key;
    int scancode;
    int action;
    int mods;
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
    };
};

} //utils
} //glre

#endif // EVENTS_H
