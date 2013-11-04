
#include <cassert>
#include <string>
#include <iomanip>

#include <GLFW/glfw3.h>

#include <glow/logging.h>
#include <glow/Timer.h>

#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/Window.h>

namespace glow
{

std::set<Window*> Window::s_windows;

Window::Window()
:   m_eventHandler(nullptr)
,   m_context(nullptr)
,   m_quitOnDestroy(true)
,   m_mode(WindowMode)
,   m_title("")
,   m_timer(nullptr)
,   m_swapts(0.0)
,   m_swaps(0)
,   m_window(nullptr)
{
    s_windows.insert(this);
}

Window::~Window()
{
    s_windows.erase(s_windows.find(this));

    assert(nullptr == m_eventHandler);
    delete m_timer;    
}

WindowEventHandler * Window::eventHandler() const
{
    return m_eventHandler;
}

Context * Window::context() const
{
    return m_context;
}

int Window::width() const
{
    return m_size.x;
}

int Window::height() const
{
    return m_size.y;
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
    const bool result = m_context->create(format);
    if (!result)
    {
        delete m_context;
        m_context = nullptr;

        if (m_eventHandler)
        {
            delete m_eventHandler;
            m_eventHandler = nullptr;
        }
        return false;
    }
    else
        promoteContext();

    m_title = title;

    m_size.x = width;
    m_size.y = height;

    m_window = m_context->window();
    if (!m_window)
    {
        fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }
    glfwSetWindowSize(m_window, m_size.x, m_size.y);
    glfwSetWindowTitle(m_window, m_title.c_str());

    glfwGetWindowSize(m_window, &width, &height);

    assert(m_size.x == width);
    assert(m_size.y == height);

    promoteContext();
    return true;
}

bool Window::quitsOnDestroy() const
{
    return m_quitOnDestroy;
}

void Window::close()
{
    onClose();
}

void Window::promoteContext()
{
    if(!m_eventHandler)
        return;

    m_context->makeCurrent();

    m_eventHandler->initializeEvent(*this);
    m_eventHandler->resizeEvent(*this, width(), height());

    m_context->doneCurrent();
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
        return windowed();
    case WindowMode:
        return fullScreen();
    }
}

void Window::assign(WindowEventHandler * eventHandler)
{
    if (eventHandler == m_eventHandler)
        return;

    if (m_eventHandler)
        delete m_eventHandler;

    m_eventHandler = eventHandler;

    if (!m_eventHandler)
        return;

    if (!m_context)
        return;

    promoteContext();
}

int Window::run()
{
    while (!s_windows.empty())
    {
        for (Window * window : s_windows)
            window->onIdle();

        glfwPollEvents();
    };
    glfwTerminate();
    return 0;
}

void Window::quit(const int code)
{

}

void Window::repaint()
{
    onRepaint();
}

void Window::onRepaint()
{
    if (!m_timer)
    {
        m_timer = new Timer(true, false);
        m_swapts = 0.0;
    }

    m_context->makeCurrent();

    if (m_eventHandler)
        m_eventHandler->paintEvent(*this);

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
    m_context->doneCurrent();
}

void Window::onResize()
{
    if (!m_context || !m_eventHandler)
        return;

    m_context->makeCurrent();
    m_eventHandler->resizeEvent(*this, width(), height());
    m_context->doneCurrent();
}

void Window::onIdle()
{
    if (!m_context || !m_eventHandler)
        return;

    if (!m_eventHandler)
        return;
    
    m_context->makeCurrent();
    m_eventHandler->idleEvent(*this);
    m_context->doneCurrent();
}

void Window::onClose()
{
    if (m_context)
    {
        if (m_eventHandler)
        {
            m_context->makeCurrent();
            
            m_eventHandler->deinitializeEvent(*this);
            delete m_eventHandler;
            m_eventHandler = nullptr;

            m_context->doneCurrent();
        }
        m_context->release();

        delete m_context;
        m_context = nullptr;
        m_window = nullptr;
    }

    if (m_quitOnDestroy)
        quit(0);
}

bool Window::onKeyPress(const int key)
{
    KeyEvent kpe(KeyEvent::KeyPressEvent, key);

    m_keysPressed.insert(kpe.key());

    if (kpe.isDiscarded())
    {
        m_context->makeCurrent();
        m_eventHandler->keyPressEvent(*this, kpe);
        m_context->doneCurrent();
    }
    return kpe.isAccepted();
}

bool Window::onKeyRelease(const int key)
{
    KeyEvent kre(KeyEvent::KeyReleaseEvent, key);

    switch (kre.key())
    {
    case GLFW_KEY_ESCAPE:
        kre.accept();
        close();
        break;

    case GLFW_KEY_ENTER:
        if (m_keysPressed.find(GLFW_KEY_LEFT_ALT) != m_keysPressed.cend())
        {
            kre.accept();
            toggleMode();
        }
        break;

    default: 
        break;
    }

    const auto f = m_keysPressed.find(kre.key());
    if (f != m_keysPressed.cend())
        m_keysPressed.erase(f);

    if (kre.isDiscarded())
    {   
        m_context->makeCurrent();
        m_eventHandler->keyReleaseEvent(*this, kre);
        m_context->doneCurrent();
    }
    return kre.isAccepted();
}

} // namespace glow
