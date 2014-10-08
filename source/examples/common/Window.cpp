#include <common/Window.h>

#include <cassert>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/base/baselogging.h>

#include <common/Context.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <common/WindowEventDispatcher.h>


using namespace glm;

std::set<Window *> Window::s_instances;

const std::set<Window *> & Window::instances()
{
    return s_instances;
}

Window::Window()
:   m_context(nullptr)
,   m_window(nullptr)
,   m_activeEventQueue  (&m_eventQueue[0])
,   m_inactiveEventQueue(&m_eventQueue[1])
,   m_quitOnDestroy(true)
,   m_mode(Mode::Windowed)
{
    s_instances.insert(this);
}

Window::~Window()
{
    s_instances.erase(this);

    if (hasPendingEvents())
    {
        clearEventQueue();
        std::swap(m_activeEventQueue, m_inactiveEventQueue);
        clearEventQueue();
    }

    if (m_context)
    {
        finalizeEventHandler();
        WindowEventDispatcher::deregisterWindow(this);
        destroyContext();
    }

    if (s_instances.empty())
        MainLoop::quit(0);
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

ivec2 Window::size() const
{
    if (!m_window)
        return ivec2();

    int w, h;
    glfwGetWindowSize(m_window, &w, &h);
    return ivec2(w, h);
}

ivec2 Window::position() const
{
    if (!m_window)
        return ivec2();

    int x, y;
    glfwGetWindowPos(m_window, &x, &y);
    return ivec2(x, y);
}

ivec2 Window::framebufferSize() const
{
    if (!m_window)
        return ivec2();

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    return ivec2(w, h);
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

void Window::setQuitOnDestroy(const bool enable)
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
        globjects::fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }

    WindowEventDispatcher::registerWindow(this);
    initializeEventHandler();

    m_windowedModeSize = ivec2(width, height);

    return true;
}

bool Window::createContext(const ContextFormat & format, int width, int height, GLFWmonitor * monitor)
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
    if (!m_eventHandler)
        return;

    m_context->makeCurrent();
    m_eventHandler->initialize(*this);
    m_context->doneCurrent();

    queueEvent(new ResizeEvent(size()));
    queueEvent(new ResizeEvent(framebufferSize(), true));
}

void Window::finalizeEventHandler()
{
    if (!m_eventHandler)
        return;

    m_context->makeCurrent();
    m_eventHandler->finalize(*this);
    m_context->doneCurrent();
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

void Window::setMode(Mode mode)
{
    if (mode == m_mode)
        return;

    const bool goFS(mode == Mode::FullScreen);

    GLFWmonitor * monitor = goFS ? glfwGetPrimaryMonitor() : nullptr;
    if (goFS && !monitor)
        return;

    clearEventQueue();


    int w(-1);
    int h(-1);

    if (goFS)
    {
        m_windowedModeSize = size();
        
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
        w = mode->width;
        h = mode->height;
    }
    else
    {
        w = m_windowedModeSize.x;
        h = m_windowedModeSize.y;
    }

    ContextFormat format = m_context->format();

    finalizeEventHandler();
    WindowEventDispatcher::deregisterWindow(this);
    destroyContext();

    if (!createContext(format, w, h, monitor))
        return;

    WindowEventDispatcher::registerWindow(this);
    initializeEventHandler();

    m_mode = mode;
}

void Window::fullScreen()
{
    setMode(Mode::FullScreen);
}

void Window::windowed()
{
    setMode(Mode::Windowed);
}

bool Window::isFullScreen() const
{
    return Mode::FullScreen == m_mode;
}

bool Window::isWindowed() const
{
    return Mode::Windowed == m_mode;
}

void Window::toggleMode()
{
    switch (m_mode)
    {
    case Mode::FullScreen:
        windowed();
        return;

    case Mode::Windowed:
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
        m_eventHandler->idle(*this);
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

    m_inactiveEventQueue->push_back(event);
}

bool Window::hasPendingEvents()
{
    return !m_inactiveEventQueue->empty();
}

void Window::processEvents()
{
    if (m_inactiveEventQueue->empty() || !m_context)
        return;

    std::swap(m_activeEventQueue, m_inactiveEventQueue);

    // note: if glfw fixes it's resize event behavior, the following workaround might be obsolete:
    // arg: only allow remove all Resize and FramebufferReszie Events, and only use the last ones
    
    ResizeEvent * resize(nullptr);
    ResizeEvent * fboresize(nullptr);

    for (auto e : *m_activeEventQueue)
    {
        switch (e->type())
        {
        case WindowEvent::Type::Resize:
            resize = dynamic_cast<ResizeEvent *>(e);
            break;

        case WindowEvent::Type::FrameBufferResize:
            fboresize = dynamic_cast<ResizeEvent *>(e);
            break;

        default:
            break;
        }
    }

    // continue with event processing, but ignore all resize events, except the last ones

    m_context->makeCurrent();

    while (!m_activeEventQueue->empty())
    {
        WindowEvent * event = m_activeEventQueue->front();
        m_activeEventQueue->erase(m_activeEventQueue->begin());

        if ((event->type() == WindowEvent::Type::Resize && event != resize)
         || (event->type() == WindowEvent::Type::FrameBufferResize && event != fboresize))
        {
            delete event;
            continue;
        }

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
        m_eventHandler->handleEvent(event);

    postprocessEvent(event);
}

void Window::postprocessEvent(WindowEvent & event)
{
    switch (event.type())
    {
    case WindowEvent::Type::Paint:
        swap();
        break;

    case WindowEvent::Type::Close:
        if (!event.isAccepted())
            destroy();
        break;

    default:
        break;
    }
}

void Window::clearEventQueue()
{
    while (!m_activeEventQueue->empty())
    {
        delete m_activeEventQueue->front();
        m_activeEventQueue->erase(m_activeEventQueue->begin());
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
