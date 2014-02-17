#include <glowwindow/WindowEventHandler.h>

#include <glowwindow/events.h>

namespace glowwindow
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

        case WindowEvent::FrameBufferResize:
            framebufferResizeEvent(static_cast<ResizeEvent&>(event));
            break;

        case WindowEvent::Move:
            moveEvent(static_cast<MoveEvent&>(event));
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

        case WindowEvent::Focus:
            focusEvent(static_cast<FocusEvent&>(event));
            break;

        case WindowEvent::Iconify:
            iconifyEvent(static_cast<IconifyEvent&>(event));
            break;

        case WindowEvent::Timer:
            timerEvent(static_cast<TimerEvent&>(event));
            break;

        default:
            break;
    }
}

void WindowEventHandler::initialize(Window &)
{
}

void WindowEventHandler::finalize(Window &)
{
}

void WindowEventHandler::idle(Window &)
{
}

void WindowEventHandler::resizeEvent(ResizeEvent &)
{
}

void WindowEventHandler::framebufferResizeEvent(ResizeEvent &)
{
}

void WindowEventHandler::moveEvent(MoveEvent &)
{
}

void WindowEventHandler::paintEvent(PaintEvent &)
{
}

void WindowEventHandler::keyPressEvent(KeyEvent &)
{
}

void WindowEventHandler::keyReleaseEvent(KeyEvent &)
{
}

void WindowEventHandler::mousePressEvent(MouseEvent &)
{
}

void WindowEventHandler::mouseMoveEvent(MouseEvent &)
{
}

void WindowEventHandler::mouseReleaseEvent(MouseEvent &)
{
}

void WindowEventHandler::scrollEvent(ScrollEvent &)
{
}

void WindowEventHandler::focusEvent(FocusEvent &)
{
}

void WindowEventHandler::iconifyEvent(IconifyEvent &)
{
}

void WindowEventHandler::timerEvent(TimerEvent &)
{
}

} // namespace glowwindow
