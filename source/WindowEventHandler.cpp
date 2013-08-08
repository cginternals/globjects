
#include <cassert>

#ifdef WIN32
#include <Windows.h>
#endif 

#include <glow/Log.h>
#include <glow/WindowEventHandler.h>

namespace glow
{

WindowEventHandler::WindowEventHandler()
{
}

WindowEventHandler::~WindowEventHandler()
{
}

void WindowEventHandler::initializeEvent()
{
}

void WindowEventHandler::deinitializeEvent()
{
}

void WindowEventHandler::resizeEvent(
    const unsigned int width
,   const unsigned int height)
{
}

void WindowEventHandler::paintEvent()
{
}

void WindowEventHandler::idleEvent()
{
}

} // namespace glow
