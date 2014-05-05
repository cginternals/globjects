#include <glowwindow/Window.h>

#include <cassert>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glowbase/baselogging.h>

#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include "WindowEventDispatcher.h"

namespace glowwindow
{

std::set<Window*> Window::s_instances;

const std::set<Window*>& Window::instances()
{
    return s_instances;
}

Window::Window()
:   m_context(nullptr)
,   m_window(nullptr)
,   m_quitOnDestroy(true)
,   m_mode(WindowMode)
{
    s_instances.insert(this);
}

Window::~Window()
{
    s_instances.erase(this);

    if (m_context)
    {
        finalizeEventHandler();
        WindowEventDispatcher::deregisterWindow(this);
        destroyContext();
    }

    if (s_instances.empty())
    {
        MainLoop::quit(0);
    }
}

WindowEventHandler * Window::eventHandler()
{
    return m_eventHandler.get();
}

const WindowEventHandler * Window::eventHandler() const
{
    return m_eventHandler.get();
}

Context * Window::context() const
{
    return m_context;
}

int Window::width() const
{
    return size().x;
}

int Window::height() const
{
    return size().y;
}

glm::ivec2 Window::size() const
{
    if (!m_window)
        return glm::ivec2();

    int w, h;
    glfwGetWindowSize(m_window, &w, &h);
    return glm::ivec2(w, h);
}

glm::ivec2 Window::position() const
{
    if (!m_window)
        return glm::ivec2();

    int x, y;
    glfwGetWindowPos(m_window, &x, &y);
    return glm::ivec2(x, y);
}

glm::ivec2 Window::framebufferSize() const
{
    if (!m_window)
        return glm::ivec2();

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    return glm::ivec2(w, h);
}

void Window::setTitle(const std::string & title)
{
    if (!m_window)
        return;

    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

const std::string & Window::title() const
{
    return m_title;
}

void Window::quitOnDestroy(const bool enable)
{
    m_quitOnDestroy = enable;
}

bool Window::create(const ContextFormat & format, const std::string & title, int width, int height)
{
    if (create(format, width, height))
    {
        setTitle(title);
        return true;
    }

    return false;
}

bool Window::create(const ContextFormat & format, int width, int height)
{
    assert(nullptr == m_context);

    if (!createContext(format, width, height))
    {
        glow::fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }

    WindowEventDispatcher::registerWindow(this);
    initializeEventHandler();

    m_windowedModeSize = glm::ivec2(width, height);

    return true;
}

bool Window::createContext(const ContextFormat & format, int width, int height, GLFWmonitor* monitor)
{
    assert(nullptr == m_context);

    m_context = new Context();

    if (m_context->create(format, width, height, monitor))
    {
        m_window = m_context->window();
    }
    else
    {
        delete m_context;
        m_context = nullptr;
        m_window = nullptr;

        return false;
    }

    return true;
}

void Window::destroyContext()
{
    m_context->release();
    delete m_context;

    m_context = nullptr;
    m_window = nullptr;
}

void Window::initializeEventHandler()
{
    if (m_eventHandler)
    {
        m_context->makeCurrent();
        m_eventHandler->initialize(*this);
        m_context->doneCurrent();

        queueEvent(new ResizeEvent(size()));
        queueEvent(new ResizeEvent(framebufferSize(), true));
    }
}

void Window::finalizeEventHandler()
{
    if (m_eventHandler)
    {
        m_context->makeCurrent();
        m_eventHandler->finalize(*this);
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
    if (WindowMode != m_mode)
        return;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor)
        return;

    m_windowedModeSize = size();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int w = mode->width;
    int h = mode->height;

    ContextFormat format = m_context->format();

    finalizeEventHandler();
    WindowEventDispatcher::deregisterWindow(this);
    destroyContext();

    if (createContext(format, w, h, monitor))
    {
        WindowEventDispatcher::registerWindow(this);
        initializeEventHandler();

        m_mode = FullScreenMode;
    }
}

void Window::windowed()
{
    if (FullScreenMode != m_mode)
        return;

    int w = m_windowedModeSize.x;
    int h = m_windowedModeSize.y;

    ContextFormat format = m_context->format();

    finalizeEventHandler();
    WindowEventDispatcher::deregisterWindow(this);
    destroyContext();


    if (createContext(format, w, h, nullptr))
    {
        WindowEventDispatcher::registerWindow(this);
        initializeEventHandler();

        m_mode = WindowMode;
    }
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

    initializeEventHandler();
}

void Window::resize(int width, int height)
{
    if (!m_window)
        return;

    glfwSetWindowSize(m_window, width, height);
}

int Window::inputMode(int mode) const
{
    if (!m_window)
        return -1;

    return glfwGetInputMode(m_window, mode);
}

void Window::setInputMode(int mode, int value)
{
    if (!m_window)
        return;

    glfwSetInputMode(m_window, mode, value);
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
    {
        m_eventHandler->idle(*this);
    }
}

void Window::swap()
{
    m_context->swap();
}

void Window::destroy()
{
    finalizeEventHandler();
    destroyContext();

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

    postprocessEvent(event);
}

void Window::postprocessEvent(WindowEvent & event)
{
    switch (event.type())
    {
        case WindowEvent::Paint:
            swap();
            break;
        case WindowEvent::Close:
            if (!event.isAccepted())
                destroy();
            break;
        default:
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
