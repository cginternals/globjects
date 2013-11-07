
#include <cassert>

#include <unordered_map>

#include <GLFW/glfw3.h>

#include <glow/logging.h>
#include <glowwindow/KeyEvent.h>


namespace glow
{

WindowEvent::WindowEvent(Type type)
: m_type(type)
, m_accepted(false)
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

bool WindowEvent::isDiscarded() const
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

void WindowEvent::discard()
{
    m_accepted = false;
}



KeyEvent::KeyEvent(int key, int scanCode, int action, int modifiers)
: WindowEvent(action == GLFW_RELEASE ? KeyRelease : KeyPress)
, m_key(key)
, m_scanCode(scanCode)
, m_action(action)
, m_modifiers(modifiers)
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


ResizeEvent::ResizeEvent(int width, int height)
: WindowEvent(Resize)
, m_width(width)
, m_height(height)
{
}

int ResizeEvent::width() const
{
    return m_width;
}

int ResizeEvent::height() const
{
    return m_height;
}

} // namespace glow
