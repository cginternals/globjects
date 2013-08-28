
#include <cassert>
#include <string>
#include <iomanip>

#ifdef WIN32
#include "WindowsWindow.h"
#elif __APPLE__

#else
#include "X11Window.h"
#endif

#include <glow/logging.h>
#include <glow/Context.h>
#include <glow/Timer.h>
#include <glow/WindowEventHandler.h>
#include <glow/Window.h>

namespace glow
{

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
#ifdef WIN32
    m_window = new WindowsWindow(*this);
#elif __APPLE__

#else
    m_window = new X11Window(*this);
#endif
}

Window::~Window()
{
    assert(nullptr == m_eventHandler);

    delete m_window;
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

int Window::handle() const
{
    return m_window->handle();
}

int Window::width() const
{
    return m_window->width();
}

int Window::height() const
{
    return m_window->height();
}

void Window::setQuitOnDestroy(const bool enable)
{
    m_quitOnDestroy = enable;
}

bool Window::create(
    const ContextFormat & format
,   const std::string & title
,   const int width
,   const int height)
{
    assert(nullptr == m_context);

    m_title = title;

    if (!m_window->create(format, title, width, height))
    {
        fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }
    assert(m_window->width()  == width);
    assert(m_window->height() == height);

    m_context = new Context();
    const bool result = m_context->create(handle(), format);
    if (!result)
    {
    	delete m_context;
	    m_context = nullptr;
    }
    else
	    promoteContext();
    
    return result;
}

bool Window::quitsOnDestroy() const
{
    return m_quitOnDestroy;
}

void Window::close()
{
    m_window->close();
}

void Window::promoteContext()
{
    if(!m_context || !m_eventHandler)
        return;

    m_context->makeCurrent();

    m_eventHandler->initializeEvent(*this);
    m_eventHandler->resizeEvent(*this, width(), height());

    m_context->doneCurrent();
}

void Window::show()
{
    m_window->show();
}

void Window::hide()
{
    m_window->hide();
}

void Window::fullScreen()
{
    if (WindowMode != m_mode)
        return;

    m_mode = TransitionMode;
    m_window->fullScreen();
    m_mode = FullScreenMode;
}

void Window::windowed()
{
    if (FullScreenMode != m_mode)
        return;

    m_mode = TransitionMode;
    m_window->windowed();
    m_mode = WindowMode;
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
#ifdef WIN32
    return WindowsWindow::run();
#elif __APPLE__

#else
    return X11Window::run();
#endif
}

void Window::quit(const int code)
{
#ifdef WIN32
    return WindowsWindow::quit(code);
#elif __APPLE__

#else
    return X11Window::quit(code);
#endif
}

void Window::repaint()
{
    m_window->repaint();
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

        m_window->setTitle(fpss.str());

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
    if (m_eventHandler)
        m_eventHandler->idleEvent(*this);
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
    }

    m_window->destroy();

    if (m_quitOnDestroy)
        quit(0);
}

bool Window::onKeyPress(const unsigned short key)
{
    KeyEvent kpe(KeyEvent::KeyPressEvent, key);

    m_keysPressed.insert(kpe.key());

    if (kpe.isDiscarded())
        m_eventHandler->keyPressEvent(*this, kpe);

    return kpe.isAccepted();
}

bool Window::onKeyRelease(const unsigned short key)
{
    KeyEvent kre(KeyEvent::KeyReleaseEvent, key);

    switch (kre.key())
    {
    case KeyEvent::KeyEscape:
        kre.accept();
        close();
        break;

    case KeyEvent::KeyReturn:
        if (m_keysPressed.find(KeyEvent::KeyAlt) != m_keysPressed.cend())
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
        m_eventHandler->keyReleaseEvent(*this, kre);

    return kre.isAccepted();
}

} // namespace glow
