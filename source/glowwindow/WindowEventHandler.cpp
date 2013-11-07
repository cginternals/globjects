
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
        case WindowEvent::Resize: {
            ResizeEvent* e = dynamic_cast<ResizeEvent*>(event);
            resizeEvent(window, e->width(), e->height());
            break; }
        case WindowEvent::Paint:
            paintEvent(window);
            break;
        case WindowEvent::KeyPress: {
            KeyEvent* e = dynamic_cast<KeyEvent*>(event);
            keyPressEvent(window, *e);
            break; }
        case WindowEvent::KeyRelease: {
            KeyEvent* e = dynamic_cast<KeyEvent*>(event);
            keyReleaseEvent(window, *e);
            break; }
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

} // namespace glow
