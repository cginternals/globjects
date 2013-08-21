
#include <glow/Window.h>

#include "AbstractNativeWindow.h"

namespace glow 
{

AbstractNativeWindow::AbstractNativeWindow(Window & window)
:   m_window(window)
{
}

AbstractNativeWindow::~AbstractNativeWindow()
{
}

WindowEventHandler * AbstractNativeWindow::eventHandler()
{
    return m_window.eventHandler();
}

Context * AbstractNativeWindow::context()
{
    return m_window.context();
}

void AbstractNativeWindow::onClose()
{
    return m_window.onClose();
}

void AbstractNativeWindow::onIdle()
{
    return m_window.onIdle();
}

void AbstractNativeWindow::onRepaint()
{
    return m_window.onRepaint();
}

void AbstractNativeWindow::onResize()
{
    return m_window.onResize();
}

bool AbstractNativeWindow::onKeyPress(const unsigned short key)
{
    return m_window.onKeyPress(key);
}

bool AbstractNativeWindow::onKeyRelease(const unsigned short key)
{
    return m_window.onKeyRelease(key);
}

bool AbstractNativeWindow::isFullScreen() const
{
    return m_window.isFullScreen();
}

bool AbstractNativeWindow::isWindowed() const
{
    return m_window.isWindowed();
}

} // namespace glow
