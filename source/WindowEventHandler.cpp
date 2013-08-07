
#include <cassert>

#ifdef WIN32
#include <Windows.h>
#endif 

#include <glow/Log.h>
#include <glow/Window.h>
#include <glow/WindowEventHandler.h>

namespace glow
{

WindowEventHandler::WindowEventHandler()
:   m_window(nullptr)
{
}

WindowEventHandler::~WindowEventHandler()
{
    // WindowEventHandler should not be attached to a Window when destructed.
    assert(!m_window);
}

void WindowEventHandler::attachEvent(Window * window)
{
    m_window = window;
}
 
void WindowEventHandler::dettachEvent(Window * window)
{
    assert(m_window == window);
    m_window = nullptr;
}

void WindowEventHandler::activateEvent()
{

}

void WindowEventHandler::minimizeEvent()
{

}

void WindowEventHandler::closeEvent()
{
    assert(m_window);
}

void WindowEventHandler::destroyEvent()
{
    PostQuitMessage(0);
}

void WindowEventHandler::resizeEvent(
    const unsigned int width
,   const unsigned int height)
{
}

} // namespace glow
