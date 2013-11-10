
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/KeyEvent.h>

namespace glow
{

WindowEventHandler::WindowEventHandler()
{
}

WindowEventHandler::~WindowEventHandler()
{
}

void WindowEventHandler::handleEvent(Window & window, WindowEvent * event)
{
    switch (event->type())
    {
    case WindowEvent::Resize: 
        {
            ResizeEvent * e = static_cast<ResizeEvent*>(event);
            resizeEvent(window, e->width(), e->height());
        }
        break;
        
    case WindowEvent::Paint:
        paintEvent(window);
        break;

    case WindowEvent::KeyPress: 
        keyPressEvent(window, *static_cast<KeyEvent*>(event));
        break;

    case WindowEvent::KeyRelease: 
        keyReleaseEvent(window, *static_cast<KeyEvent*>(event));
        break;

    case WindowEvent::MousePress:
        mousePressEvent(window, *static_cast<MouseEvent*>(event));
        break;

    case WindowEvent::MouseRelease:
        mouseReleaseEvent(window, *static_cast<MouseEvent*>(event));
        break;

    case WindowEvent::MouseMove:
        mouseMoveEvent(window, *static_cast<MouseEvent*>(event));
        break;

    case WindowEvent::Scroll:
        scrollEvent(window, *static_cast<ScrollEvent*>(event));
        break;
    }
}

void WindowEventHandler::initialize(Window &)
{
}

void WindowEventHandler::finalize(Window &)
{
}

void WindowEventHandler::resizeEvent(
    Window &
,   const unsigned int
,   const unsigned int)
{
}

void WindowEventHandler::paintEvent(Window &)
{
}

void WindowEventHandler::idleEvent(Window &)
{
}

void WindowEventHandler::keyPressEvent(
    Window &
,   KeyEvent &)
{
}

void WindowEventHandler::keyReleaseEvent(
    Window &
,   KeyEvent &)
{
}

void WindowEventHandler::mousePressEvent(
    Window & window
,   MouseEvent & event)
{
}

void WindowEventHandler::mouseMoveEvent(
    Window & window
,   MouseEvent & event)
{
}

void WindowEventHandler::mouseReleaseEvent(
    Window & window
    , MouseEvent & event)
{
}

void WindowEventHandler::scrollEvent(
    Window & window
,   ScrollEvent & event)
{
}

} // namespace glow
