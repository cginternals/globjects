#pragma once

#include <GLFW/glfw3.h> ///< promote keys

#include <glowwindow/glowwindow.h>

namespace glow 
{

class GLOWWINDOW_API WindowEvent
{
public:
    enum Type
    {
        KeyPress
    ,   KeyRelease
    ,   MousePress
    ,   MouseRelease
    ,   Scroll
    ,   Resize
    ,   Move
    ,   Hover
    ,   Close
    ,   Focus
    ,   Iconify
    ,   Paint
    };

    WindowEvent(Type type);
    virtual ~WindowEvent();

    Type type() const;

    bool isAccepted() const;
    bool isDiscarded() const;

    void setAccepted(bool accepted);
    void accept();
    void discard();
protected:
    Type m_type;
    bool m_accepted;
};

class GLOWWINDOW_API KeyEvent : public WindowEvent
{
public:
    KeyEvent(int key, int scanCode, int action, int modifiers);

    int key() const;
    int scanCode() const;
    int action() const;
    int modifiers() const;

protected:
    int m_key;
    int m_scanCode;
    int m_action;
    int m_modifiers;
};

class GLOWWINDOW_API MouseEvent : public WindowEvent
{
public:
    MouseEvent(int button, int scanCode, int action, int modifiers);

    int button() const;
    int action() const;
    int modifiers() const;

protected:
    int m_button;
    int m_action;
    int m_modifiers;
};

class GLOWWINDOW_API ResizeEvent : public WindowEvent
{
public:
    ResizeEvent(int width, int height);

    int width() const;
    int height() const;

protected:
    int m_width;
    int m_height;
};

} // namespace glow
