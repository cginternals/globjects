
#include <cassert>
#include <string>
#include <iomanip>

#include <GLFW/glfw3.h>

#include <glow/logging.h>
#include <glow/Timer.h>

#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/Window.h>
#include <glowwindow/KeyEvent.h>

#include "WindowEventDispatcher.h"

namespace glow
{

std::set<Window*> Window::s_windows;

Window::Window()
:   m_context(nullptr)
,   m_quitOnDestroy(true)
,   m_mode(WindowMode)
,   m_title("")
,   m_timer(nullptr)
,   m_swapts(0.0)
,   m_swaps(0)
,   m_window(nullptr)
,   m_width(0)
,   m_height(0)
{
    s_windows.insert(this);
}

Window::~Window()
{
    s_windows.erase(this);
    WindowEventDispatcher::deregisterWindow(this);

    if (s_windows.empty())
        quit(0);

    delete m_timer;    
}

WindowEventHandler * Window::eventHandler() const
{
    return const_cast<WindowEventHandler*>(m_eventHandler.get());
}

Context * Window::context() const
{
    return m_context;
}

int Window::width() const
{
    return m_width;
}

int Window::height() const
{
    return m_height;
}

void Window::setQuitOnDestroy(const bool enable)
{
    m_quitOnDestroy = enable;
}

bool Window::create(
    const ContextFormat & format
,   const std::string & title
,   int width
,   int height)
{
    assert(nullptr == m_context);

    m_context = new Context();
    if (!m_context->create(format))
    {
        delete m_context;
        m_context = nullptr;

        return false;
    }

    m_window = m_context->window();
    if (!m_window)
    {
        fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }

    m_title = title;

    m_width = width;
    m_height = height;

    glfwSetWindowSize(m_window, m_width, m_height);
    glfwSetWindowTitle(m_window, m_title.c_str());

    WindowEventDispatcher::registerWindow(this);

    promoteContext();

    return true;
}

void Window::promoteContext()
{
    if (m_eventHandler)
    {
         m_context->makeCurrent();
         m_eventHandler->initialize(*this);
		 m_eventHandler->resizeEvent(*this, m_width, m_height);
         m_context->doneCurrent();
    }
}

bool Window::quitsOnDestroy() const
{
    return m_quitOnDestroy;
}

void Window::show()
{
    if (!m_window)
        return;

    glfwShowWindow(m_window);
}

void Window::hide()
{
    if (!m_window)
        return;

    glfwHideWindow(m_window);
}

void Window::fullScreen()
{
    //if (WindowMode != m_mode)
    //    return;

    //m_mode = TransitionMode;
    //m_window->fullScreen();
    //m_mode = FullScreenMode;
}

void Window::windowed()
{
    //if (FullScreenMode != m_mode)
    //    return;

    //m_mode = TransitionMode;
    //m_window->windowed();
    //m_mode = WindowMode;
}

bool Window::isFullScreen() const
{
    return FullScreenMode == m_mode;
}

bool Window::isWindowed() const
{
    return WindowMode == m_mode;
}

void Window::toggleMode()
{
    switch (m_mode)
    {
    case TransitionMode:
        return;
    case FullScreenMode:
        windowed();
        return;
    case WindowMode:
        fullScreen();
        return;
    }
}

void Window::assign(WindowEventHandler * eventHandler)
{
    if (eventHandler == m_eventHandler)
        return;

    m_eventHandler = eventHandler;

    if (!m_eventHandler)
        return;

    if (!m_context)
        return;

    promoteContext();
}

bool Window::running = false;
int Window::exitCode = 0;

int Window::run()
{
    running = true;

    while (running)
    {
        for (Window * window : s_windows)
            window->idle();

        glfwPollEvents();
    };
    glfwTerminate();
    return exitCode;
}

void Window::quit(const int code)
{
    exitCode = code;
    running = false;
}

void Window::repaint()
{
    WindowEvent e(WindowEvent::Paint);
    processEvent(&e);
}

void Window::close()
{
    WindowEvent e(WindowEvent::Close);
    processEvent(&e);
}

void Window::paint()
{
    if (!m_timer)
    {
        m_timer = new Timer(true, false);
        m_swapts = 0.0;
    }

    m_context->swap();

    m_timer->update();

    ++m_swaps;

    if (m_timer->elapsed() - m_swapts >= 1e+9)
    {
        const float fps = 1e+9f * static_cast<float>(static_cast<long double>
            (m_swaps) / (m_timer->elapsed() - m_swapts));

        std::stringstream fpss;
        fpss << m_title << " (" << std::fixed << std::setprecision(2) << fps << " fps)";

        glfwSetWindowTitle(m_window, fpss.str().c_str());

        m_swapts = m_timer->elapsed();
        m_swaps = 0;
    }
}

void Window::idle()
{
    if (!m_context || !m_eventHandler)
        return;

    if (!m_eventHandler)
        return;
    
    m_context->makeCurrent();
    m_eventHandler->idleEvent(*this);
    m_context->doneCurrent();
}

void Window::destroy()
{
    m_context->release();

    delete m_context;
    m_context = nullptr;
    m_window = nullptr;

    if (m_eventHandler)
        m_eventHandler->finalize(*this);

    if (m_quitOnDestroy)
        quit(0);
}

void Window::processEvent(WindowEvent* event)
{
    if (!m_context)
        return;

    m_context->makeCurrent();
    m_eventHandler->handleEvent(*this, event);

    if (!event->isAccepted())
        defaultAction(event);

    if (m_context)
        m_context->doneCurrent();
}

void Window::defaultAction(WindowEvent* event)
{
    switch (event->type())
    {
        case WindowEvent::Close:
            destroy();
            event->accept();
            break;
        case WindowEvent::Paint:
            paint();
            event->accept();
            break;
        case WindowEvent::KeyPress:
            KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);
            if (keyEvent->key() == GLFW_KEY_ESCAPE)
            {
                keyEvent->accept();
                close();
            }
            //    case GLFW_KEY_ENTER:
            //        if (kre.modifiers() & GLFW_MOD_ALT != 0)
            //        {
            //            kre.accept();
            //            toggleMode();
            //        }
            //        break;

            //    default:
            //        break;
            //    }

            break;
    }
}

} // namespace glow
