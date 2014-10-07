#include <common/WindowEventHandler.h>

#include <common/events.h>


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
    case WindowEvent::Type::Resize:
        resizeEvent(static_cast<ResizeEvent &>(event));
        break;

    case WindowEvent::Type::FrameBufferResize:
        framebufferResizeEvent(static_cast<ResizeEvent &>(event));
        break;

    case WindowEvent::Type::Move:
        moveEvent(static_cast<MoveEvent &>(event));
        break;

    case WindowEvent::Type::Paint:
        paintEvent(static_cast<PaintEvent &>(event));
        break;

    case WindowEvent::Type::KeyPress:
        keyPressEvent(static_cast<KeyEvent &>(event));
        break;

    case WindowEvent::Type::KeyRelease:
        keyReleaseEvent(static_cast<KeyEvent &>(event));
        break;

    case WindowEvent::Type::MousePress:
        mousePressEvent(static_cast<MouseEvent &>(event));
        break;

    case WindowEvent::Type::MouseRelease:
        mouseReleaseEvent(static_cast<MouseEvent &>(event));
        break;

    case WindowEvent::Type::MouseMove:
        mouseMoveEvent(static_cast<MouseEvent &>(event));
        break;

    case WindowEvent::Type::MouseEnter:
        mouseEnterEvent(static_cast<MouseEnterEvent &>(event));
        break;

    case WindowEvent::Type::MouseLeave:
        mouseLeaveEvent(static_cast<MouseLeaveEvent &>(event));
        break;

    case WindowEvent::Type::Scroll:
        scrollEvent(static_cast<ScrollEvent &>(event));
        break;

    case WindowEvent::Type::Focus:
        focusEvent(static_cast<FocusEvent &>(event));
        break;

    case WindowEvent::Type::Iconify:
        iconifyEvent(static_cast<IconifyEvent &>(event));
        break;

    case WindowEvent::Type::Timer:
        timerEvent(static_cast<TimerEvent &>(event));
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

void WindowEventHandler::mouseEnterEvent(MouseEnterEvent &)
{
}

void WindowEventHandler::mouseLeaveEvent(MouseLeaveEvent &)
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
