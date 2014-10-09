#include "events.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


WindowEvent::WindowEvent(Type type)
: m_type(type)
, m_accepted(false)
, m_window(nullptr)
{
}

WindowEvent::~WindowEvent()
{
}

WindowEvent::Type WindowEvent::type() const
{
    return m_type;
}

bool WindowEvent::isAccepted() const
{
    return m_accepted;
}

bool WindowEvent::isIgnored() const
{
    return !m_accepted;
}

void WindowEvent::setAccepted(const bool accepted)
{
    m_accepted = accepted;
}

void WindowEvent::accept()
{
    m_accepted = true;
}

void WindowEvent::ignore()
{
    m_accepted = false;
}

Window * WindowEvent::window() const
{
    return m_window;
}

void WindowEvent::setWindow(Window *window)
{
    m_window = window;
}


KeyEvent::KeyEvent(unsigned int character)
: WindowEvent(Type::KeyTyped)
, m_key(0)
, m_scanCode(0)
, m_action(0)
, m_modifiers(0)
, m_character(character)
{
}

KeyEvent::KeyEvent(int key, int scanCode, int action, int modifiers)
: WindowEvent(action == GLFW_RELEASE ? Type::KeyRelease : Type::KeyPress)
, m_key(key)
, m_scanCode(scanCode)
, m_action(action)
, m_modifiers(modifiers)
, m_character(0)
{
}

int KeyEvent::key() const
{
    return m_key;
}

int KeyEvent::scanCode() const
{
    return m_scanCode;
}

int KeyEvent::action() const
{
    return m_action;
}

int KeyEvent::modifiers() const
{
    return m_modifiers;
}

unsigned int KeyEvent::character() const
{
    return m_character;
}


ResizeEvent::ResizeEvent(const glm::ivec2 & size, bool framebuffer)
: WindowEvent(framebuffer ? Type::FrameBufferResize : Type::Resize)
, m_size(size)
{
}

const glm::ivec2 & ResizeEvent::size() const
{
    return m_size;
}

int ResizeEvent::width() const
{
    return m_size.x;
}

int ResizeEvent::height() const
{
    return m_size.y;
}


MouseEvent::MouseEvent(const glm::ivec2 & pos)
: WindowEvent(Type::MouseMove)
, m_button(-1)
, m_action(-1)
, m_modifiers(0)
, m_pos(pos)
{
}

MouseEvent::MouseEvent(const glm::ivec2 & pos, const int button, const int action, const int modifiers)
: WindowEvent(action == GLFW_RELEASE ? Type::MouseRelease : Type::MousePress)
, m_button(button)
, m_action(action)
, m_modifiers(modifiers)
, m_pos(pos)
{
}

int MouseEvent::button() const
{
    return m_button;
}

int MouseEvent::action() const
{
    return m_action;
}

int MouseEvent::modifiers() const
{
    return m_modifiers;
}

int MouseEvent::x() const
{
    return m_pos.x;
}

int MouseEvent::y() const
{
    return m_pos.y;
}

const glm::ivec2 & MouseEvent::pos() const
{
    return m_pos;
}


MouseEnterEvent::MouseEnterEvent()
: WindowEvent(Type::MouseEnter)
{
}


MouseLeaveEvent::MouseLeaveEvent()
: WindowEvent(Type::MouseLeave)
{
}


ScrollEvent::ScrollEvent(const glm::vec2 & offset, const glm::ivec2 & pos)
: WindowEvent(Type::Scroll)
, m_offset(offset)
, m_pos(pos)
{
}

const glm::vec2 & ScrollEvent::offset() const
{
    return m_offset;
}

const glm::ivec2 & ScrollEvent::pos() const
{
    return m_pos;
}


MoveEvent::MoveEvent(const glm::ivec2 & pos)
: WindowEvent(Type::Move)
, m_pos(pos)
{
}

int MoveEvent::x() const
{
    return m_pos.x;
}

int MoveEvent::y() const
{
    return m_pos.y;
}

const glm::ivec2 & MoveEvent::pos() const
{
    return m_pos;
}


PaintEvent::PaintEvent()
: WindowEvent(Type::Paint)
{
}


CloseEvent::CloseEvent()
: WindowEvent(Type::Close)
{
}


FocusEvent::FocusEvent(bool hasFocus)
: WindowEvent(Type::Focus)
, m_hasFocus(hasFocus)
{
}

bool FocusEvent::hasFocus() const
{
    return m_hasFocus;
}


IconifyEvent::IconifyEvent(bool isIconified)
: WindowEvent(Type::Iconify)
, m_isIconified(isIconified)
{
}

bool IconifyEvent::isIconified() const
{
    return m_isIconified;
}


TimerEvent::TimerEvent(int id, const Duration & elapsed)
: WindowEvent(Type::Timer)
, m_id(id)
, m_elapsed(elapsed)
{
}

int TimerEvent::id() const
{
    return m_id;
}

const TimerEvent::Duration & TimerEvent::elapsed() const
{
    return m_elapsed;
}
