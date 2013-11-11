
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

namespace glow
{

WindowEventHandler::WindowEventHandler()
{
}

WindowEventHandler::~WindowEventHandler()
{
}

void WindowEventHandler::handleEvent(WindowEvent & event)
{
    if (!event.window())
        return;

    switch (event.type())
    {
        case WindowEvent::Resize:
            resizeEvent(static_cast<ResizeEvent&>(event));
            break;

        case WindowEvent::Paint:
            paintEvent(static_cast<PaintEvent&>(event));
            break;

        case WindowEvent::KeyPress:
            keyPressEvent(static_cast<KeyEvent&>(event));
            break;

        case WindowEvent::KeyRelease:
            keyReleaseEvent(static_cast<KeyEvent&>(event));
            break;

        case WindowEvent::MousePress:
            mousePressEvent(static_cast<MouseEvent&>(event));
            break;

        case WindowEvent::MouseRelease:
            mouseReleaseEvent(static_cast<MouseEvent&>(event));
            break;

        case WindowEvent::MouseMove:
            mouseMoveEvent(static_cast<MouseEvent&>(event));
            break;

        case WindowEvent::Scroll:
            scrollEvent(static_cast<ScrollEvent&>(event));
            break;

        case WindowEvent::Idle:
            idleEvent(static_cast<IdleEvent&>(event));
            break;
    }
}

void WindowEventHandler::initialize(Window &)
{
}

void WindowEventHandler::finalize(Window &)
{
}

void WindowEventHandler::resizeEvent(ResizeEvent & event)
{
}

void WindowEventHandler::paintEvent(PaintEvent &)
{
}

void WindowEventHandler::idleEvent(IdleEvent &)
{
}

void WindowEventHandler::keyPressEvent(KeyEvent &)
{
}

void WindowEventHandler::keyReleaseEvent(KeyEvent &)
{
}

void WindowEventHandler::mousePressEvent(MouseEvent & event)
{
}

void WindowEventHandler::mouseMoveEvent(MouseEvent & event)
{
}

void WindowEventHandler::mouseReleaseEvent(MouseEvent & event)
{
}

void WindowEventHandler::scrollEvent(ScrollEvent & event)
{
}

} // namespace glow
