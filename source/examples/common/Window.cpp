#include <common/Window.h>

#include <cassert>
#include <iostream>

#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/base/baselogging.h>

#include <common/Context.h>
#include <common/ContextFormat.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <common/WindowEventDispatcher.h>


using namespace glm;
using namespace globjects;

std::set<Window *> Window::s_instances;

const std::set<Window *> & Window::instances()
{
    return s_instances;
}

int Window::init()
{
    return glfwInit();
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
    if (!create(format, width, height))
        return false;

    setTitle(title);
    return true;
}

bool Window::create(const ContextFormat & format, int width, int height)
{
    assert(nullptr == m_context);

    if (!createContext(format, true, width, height))
    {
        fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }

    WindowEventDispatcher::registerWindow(this);
    initializeEventHandler();

    m_windowedModeSize = ivec2(width, height);

    context()->makeCurrent();
    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl << std::endl;
    context()->doneCurrent();

    return true;
}

bool Window::createContext(
    const ContextFormat & format
,   bool verify
,   int width
,   int height
,   GLFWmonitor * monitor)
{
    const bool restoreInterval = m_context != nullptr;
    Context::SwapInterval interval = Context::SwapInterval::NoVerticalSyncronization;
    if (restoreInterval)
        interval = m_context->swapInterval();

    destroyContext();

    assert(!m_context);
    assert(!m_window);

    m_window = Context::create(format, verify, width, height, monitor);
    if (!m_window)
        return false;

    m_context = new Context(m_window);

    if (restoreInterval)
        m_context->setSwapInterval(interval);

    if (verify)
        m_context->format().verify(format);

    return true;
}

void Window::destroyContext()
{
    delete m_context;
    glfwDestroyWindow(m_window);

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
    assert(m_context);

    if (mode == m_mode)
        return;

    const bool goFS(mode == Mode::FullScreen);

    GLFWmonitor * monitor = goFS ? glfwGetPrimaryMonitor() : nullptr;
    if (goFS && !monitor)
        return;

    clearEventQueue();

    int w, h, x, y;
    w = h = x = y = -1;

    if (goFS)
    {
        m_windowedModeSize = size();
        m_windowedModePosition = position();
        
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
        w = mode->width;
        h = mode->height;
        x = y = 0;
    }
    else
    {
        w = m_windowedModeSize.x;
        h = m_windowedModeSize.y;
        x = m_windowedModePosition.x;
        y = m_windowedModePosition.y;
    }

    ContextFormat format = m_context->format();

    finalizeEventHandler();
    WindowEventDispatcher::deregisterWindow(this);

    if (!createContext(format, false, w, h, monitor))
        return;

    if (!goFS)
        glfwSetWindowPos(m_window, x, y);

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

void Window::toggleVSync()
{
    switch (context()->swapInterval())
    {
    case Context::SwapInterval::NoVerticalSyncronization:
        context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);
        break;

    case Context::SwapInterval::VerticalSyncronization:
    case Context::SwapInterval::AdaptiveVerticalSyncronization:
        context()->setSwapInterval(Context::SwapInterval::NoVerticalSyncronization);
        break;
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
    glfwSwapBuffers(m_window);
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
