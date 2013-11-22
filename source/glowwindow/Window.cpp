
#include <cassert>
#include <string>
#include <iomanip>

#include <GLFW/glfw3.h>

#include <glow/logging.h>
#include <glow/Timer.h>

#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/Window.h>
#include <glowwindow/events.h>

#include "WindowEventDispatcher.h"


using namespace glow;

namespace glowwindow
{

std::set<Window*> Window::s_instances;

const std::set<Window*>& Window::instances()
{
    return s_instances;
}

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
    s_instances.insert(this);
}

Window::~Window()
{
    s_instances.erase(this);
    WindowEventDispatcher::deregisterWindow(this);

    if (s_instances.empty())
        MainLoop::quit(0);

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

void Window::quitOnDestroy(const bool enable)
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
    if (!m_context->create(format, width, height))
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

    m_width = width;
    m_height = height;

    setTitle(title);

    WindowEventDispatcher::registerWindow(this);

    promoteContext();

    return true;
}

void Window::setTitle(const std::string & title)
{
    m_title = title;

    if (!m_window)
        return;

    glfwSetWindowTitle(m_window, m_title.c_str());
}

void Window::promoteContext()
{
    if (m_eventHandler)
    {
         m_context->makeCurrent();
         m_eventHandler->initialize(*this);
         m_context->doneCurrent();

         queueEvent(new ResizeEvent(glm::ivec2(m_width, m_height)));
         queueEvent(new ResizeEvent(glm::ivec2(m_width, m_height), true));
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

void Window::setEventHandler(WindowEventHandler * eventHandler)
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

void Window::resize(
    const int width
,   const int height)
{
    if (!m_window)
        return;

    glfwSetWindowSize(m_window, width, height);
}

void Window::repaint()
{
    queueEvent(new PaintEvent);
}

void Window::close()
{
    queueEvent(new CloseEvent);
}

void Window::idle()
{
    if (m_eventHandler)
        m_eventHandler->idle(*this);
}

void Window::swap()
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

void Window::destroy()
{
    m_context->release();

    delete m_context;
    m_context = nullptr;
    m_window = nullptr;

    if (m_eventHandler)
        m_eventHandler->finalize(*this);

    if (m_quitOnDestroy)
        MainLoop::quit(0);
}

GLFWwindow * Window::internalWindow() const
{
    return m_window;
}

void Window::queueEvent(WindowEvent * event)
{
    if (!event)
        return;

    m_eventQueue.push(event);
}

bool Window::hasPendingEvents()
{
    return !m_eventQueue.empty();
}

void Window::processEvents()
{
    if (m_eventQueue.empty() || !m_context)
        return;

    m_context->makeCurrent();

    while (!m_eventQueue.empty())
    {
        WindowEvent* event = m_eventQueue.front();
        m_eventQueue.pop();
        event->setWindow(this);

        processEvent(*event);

        delete event;

        if (!m_context)
        {
            clearEventQueue();
            return;
        }
    }

    m_context->doneCurrent();
}

void Window::processEvent(WindowEvent & event)
{
    if (m_eventHandler)
    {
        m_eventHandler->handleEvent(event);
    }

    finishEvent(event);
}

void Window::finishEvent(WindowEvent & event)
{
    if (event.type() == WindowEvent::Paint)
    {
        swap();
    }
    else if (!event.isAccepted())
    {
        defaultEventAction(event);
    }
}

void Window::defaultEventAction(WindowEvent & event)
{
    switch (event.type())
    {
        case WindowEvent::Close:
            destroy();
            break;

        case WindowEvent::KeyPress:
            KeyEvent& keyEvent = static_cast<KeyEvent&>(event);
            switch (keyEvent.key())
            {
                case GLFW_KEY_ESCAPE:
                    close();
                    break;
                case GLFW_KEY_ENTER:
                    if ((keyEvent.modifiers() & GLFW_MOD_ALT) != 0)
                    {
                        toggleMode();
                    }
                    break;
            }
            break;
    }
}

void Window::clearEventQueue()
{
    while (!m_eventQueue.empty())
    {
        delete m_eventQueue.front();
        m_eventQueue.pop();
    }
}

void Window::addTimer(int id, int interval, bool singleShot)
{
    WindowEventDispatcher::addTimer(this, id, interval, singleShot);
}

void Window::removeTimer(int id)
{
    WindowEventDispatcher::removeTimer(this, id);
}

} // namespace glowwindow
