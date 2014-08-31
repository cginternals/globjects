#pragma once

#include <globjects-window/keys.h>

#include <glm/glm.hpp>

#include <globjects-window/globjects-window_api.h>

namespace glowindow
{

class Window;

class GLOBJECTS_WINDOW_API WindowEvent
{
public:
    enum Type
    {
        KeyPress
    ,   KeyRelease
    ,   KeyTyped
    ,   MousePress
    ,   MouseRelease
    ,   MouseMove
    ,   MouseEnter
    ,   MouseLeave
    ,   Scroll
    ,   Resize
    ,   FrameBufferResize
    ,   Move
    ,   Close
    ,   Focus
    ,   Iconify
    ,   Paint
    ,   Timer
    };

    virtual ~WindowEvent();

    Type type() const;

    bool isAccepted() const;
    bool isIgnored() const;

    void setAccepted(bool accepted);
    void accept();
    void ignore();

    Window* window() const;
    void setWindow(Window* window);
protected:
    Type m_type;
    bool m_accepted;
    Window* m_window;

protected:
    WindowEvent(Type type);
};

class GLOBJECTS_WINDOW_API KeyEvent : public WindowEvent
{
public:
    KeyEvent(int key, int scanCode, int action, int modifiers);
    explicit KeyEvent(unsigned int character);

    int key() const;
    int scanCode() const;
    int action() const;
    int modifiers() const;
    unsigned int character() const;

protected:
    int m_key;
    int m_scanCode;
    int m_action;
    int m_modifiers;
    unsigned int m_character;
};

class GLOBJECTS_WINDOW_API MouseEvent : public WindowEvent
{
public:
    MouseEvent(const glm::ivec2 & pos);
    MouseEvent(const glm::ivec2 & pos, int button, int action, int modifiers);

    int button() const;
    int action() const;
    int modifiers() const;

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;

protected:
    int m_button;
    int m_action;
    int m_modifiers;

    glm::ivec2 m_pos;
};

class GLOBJECTS_WINDOW_API MouseEnterEvent : public WindowEvent
{
public:
    MouseEnterEvent();
};

class GLOBJECTS_WINDOW_API MouseLeaveEvent : public WindowEvent
{
public:
    MouseLeaveEvent();
};

class GLOBJECTS_WINDOW_API ScrollEvent : public WindowEvent
{
public:
    ScrollEvent(const glm::vec2 & offset, const glm::ivec2 & pos);

    const glm::vec2 & offset() const;
    const glm::ivec2 & pos() const;

protected:
    glm::vec2 m_offset;
    glm::ivec2 m_pos;
};

class GLOBJECTS_WINDOW_API MoveEvent : public WindowEvent
{
public:
    MoveEvent(const glm::ivec2 & pos);

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;

protected:
    glm::ivec2 m_pos;
};

class GLOBJECTS_WINDOW_API ResizeEvent : public WindowEvent
{
public:
    ResizeEvent(const glm::ivec2 & size, bool framebuffer = false);

    const glm::ivec2 & size() const;

    int width() const;
    int height() const;

protected:
    glm::ivec2 m_size;
};

class GLOBJECTS_WINDOW_API PaintEvent : public WindowEvent
{
public:
    PaintEvent();
};

class GLOBJECTS_WINDOW_API CloseEvent : public WindowEvent
{
public:
    CloseEvent();
};

class GLOBJECTS_WINDOW_API FocusEvent : public WindowEvent
{
public:
    FocusEvent(bool hasFocus);

    bool hasFocus() const;
protected:
    bool m_hasFocus;
};

class GLOBJECTS_WINDOW_API IconifyEvent : public WindowEvent
{
public:
    IconifyEvent(bool isIconified);

    bool isIconified() const;
protected:
    bool m_isIconified;
};

class GLOBJECTS_WINDOW_API TimerEvent : public WindowEvent
{
public:
    TimerEvent(int id);

    int id() const;
protected:
    int m_id;
};

} // namespace glowindow
