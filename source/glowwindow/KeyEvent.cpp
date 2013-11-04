
#include <cassert>

#include <unordered_map>

#include <GLFW/glfw3.h>

#include <glow/logging.h>
#include <glowwindow/KeyEvent.h>


namespace glow
{

KeyEvent::KeyEvent(
    const Type type
,   const int key)
:   m_type(type)
, m_key(key)
, m_accept(false)
{

}

KeyEvent::~KeyEvent()
{
}

int KeyEvent::key() const
{
    return m_key;
}

void KeyEvent::setAccept(const bool accept)
{
    m_accept = accept;
}

bool KeyEvent::isAccepted() const
{
    return m_accept;
}

bool KeyEvent::isDiscarded() const
{
    return !m_accept;
}

void KeyEvent::accept()
{
    m_accept = true;
}
    
void KeyEvent::discard()
{
    m_accept = false;
}


} // namespace glow
