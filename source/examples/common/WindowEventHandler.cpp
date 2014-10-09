#include <common/WindowEventHandler.h>

#include <iomanip>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>
#include <globjects/DebugMessage.h>

#include <globjects/base/File.h>
#include <globjects/base/baselogging.h>

#include <common/events.h>
#include <common/Window.h>


using namespace gl;
using namespace globjects;

WindowEventHandler::WindowEventHandler()
: m_swapElapsedTime(0.0)
, m_swapCount(0)
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
    init();
    DebugMessage::enable();

#ifdef MAC_OS
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif
}

void WindowEventHandler::finalize(Window &)
{
    detachAllObjects();
}

void WindowEventHandler::idle(Window & window)
{
    window.repaint();
}

void WindowEventHandler::resizeEvent(ResizeEvent &)
{
}

void WindowEventHandler::framebufferResizeEvent(ResizeEvent & event)
{
    glViewport(0, 0, event.width(), event.height());
}

void WindowEventHandler::moveEvent(MoveEvent &)
{
}

void WindowEventHandler::paintEvent(PaintEvent & event)
{
    computeFps(static_cast<PaintEvent &>(event));
}

void WindowEventHandler::keyPressEvent(KeyEvent & event)
{
    switch (event.key())
    {
    case GLFW_KEY_ESCAPE:
        event.window()->close();
        break;

    case GLFW_KEY_ENTER:
        if ((event.modifiers() & GLFW_MOD_ALT) == 0)
            break;
        // fall through

    case GLFW_KEY_F11:
        event.window()->toggleMode();
        break;

    case GLFW_KEY_F10:
        event.window()->toggleVSync();
        break;

    default:
        break;
    }
}

void WindowEventHandler::keyReleaseEvent(KeyEvent & event)
{
    if (GLFW_KEY_F5 == event.key())
        File::reloadAll();
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

void WindowEventHandler::timerEvent(TimerEvent & event)
{
    event.window()->repaint();
}

namespace
{
    bool startsWith(const std::string & str, const std::string str2)
    {
        return str.compare(0, str2.length(), str2) == 0;
    }
}

void WindowEventHandler::computeFps(PaintEvent & event)
{
    m_timer.update();

    ++m_swapCount;

    if (m_timer.elapsed().count() - m_swapElapsedTime >= 1e+9)
    {
        const float fps = 1e+9f * static_cast<float>(static_cast<long double>(m_swapCount) / (m_timer.elapsed().count() - m_swapElapsedTime));

        std::string title = event.window()->title();
        if (!startsWith(title, m_baseTitle) || m_baseTitle.length() == 0)
            m_baseTitle = title;

        std::stringstream stream;
        stream << m_baseTitle << " (" << std::fixed << std::setprecision(2) << fps << " fps)";

        event.window()->setTitle(stream.str());

        m_swapElapsedTime = static_cast<long double>(m_timer.elapsed().count());
        m_swapCount = 0;
    }
}
