#pragma once

#include <chrono>

#include <glm/glm.hpp>


class Window;


class WindowEvent
{
public:
    enum class Type
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

    Window * window() const;
    void setWindow(Window * window);

protected:
    WindowEvent(Type type);

protected:
    Type m_type;
    bool m_accepted;

    Window * m_window;
};


class KeyEvent : public WindowEvent
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


class MouseEvent : public WindowEvent
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


class MouseEnterEvent : public WindowEvent
{
public:
    MouseEnterEvent();
};


class MouseLeaveEvent : public WindowEvent
{
public:
    MouseLeaveEvent();
};


class ScrollEvent : public WindowEvent
{
public:
    ScrollEvent(const glm::vec2 & offset, const glm::ivec2 & pos);

    const glm::vec2 & offset() const;
    const glm::ivec2 & pos() const;

protected:
    glm::vec2 m_offset;
    glm::ivec2 m_pos;
};


class MoveEvent : public WindowEvent
{
public:
    MoveEvent(const glm::ivec2 & pos);

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;

protected:
    glm::ivec2 m_pos;
};


class ResizeEvent : public WindowEvent
{
public:
    ResizeEvent(const glm::ivec2 & size, bool framebuffer = false);

    const glm::ivec2 & size() const;

    int width() const;
    int height() const;

protected:
    glm::ivec2 m_size;
};


class PaintEvent : public WindowEvent
{
public:
    PaintEvent();
};


class CloseEvent : public WindowEvent
{
public:
    CloseEvent();
};

class FocusEvent : public WindowEvent
{
public:
    FocusEvent(bool hasFocus);

    bool hasFocus() const;

protected:
    bool m_hasFocus;
};


class IconifyEvent : public WindowEvent
{
public:
    IconifyEvent(bool isIconified);

    bool isIconified() const;

protected:
    bool m_isIconified;
};


class TimerEvent : public WindowEvent
{
public:
    using Duration = std::chrono::duration<double, std::milli>;

    TimerEvent(int id, const Duration & elapsed);

    int id() const;
    const Duration & elapsed() const;

protected:
    int m_id;
    Duration m_elapsed;
};
