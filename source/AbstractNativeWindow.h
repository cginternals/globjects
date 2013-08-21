#pragma once

#include <glow/KeyEvent.h>

namespace glow 
{
class WindowEventHandler;
class ContextFormat;
class Context;
class Window;

class AbstractNativeWindow
{
public:
    /** The window is used to obtain context and eventhandler access.
    */
    AbstractNativeWindow(Window & window);
    virtual ~AbstractNativeWindow();

    virtual bool create(
        const ContextFormat & format
    ,   const std::string & title
    ,   const unsigned int width
    ,   const unsigned int height) = 0;

    virtual void close() = 0;
    virtual void destroy() = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual int handle() const = 0;

    virtual void repaint() = 0;

    virtual void show() = 0;
    virtual void hide() = 0;

    virtual void fullScreen() = 0;
    virtual void windowed() = 0;

    virtual void setTitle(const std::string & title) = 0;

protected:    
    WindowEventHandler * eventHandler();
    Context * context();

    bool isFullScreen() const;
    bool isWindowed() const;

    virtual void onClose();
    virtual void onIdle();
    virtual void onRepaint();
    virtual void onResize();

    virtual bool onKeyPress(const unsigned short key);
    virtual bool onKeyRelease(const unsigned short key);

protected:
    Window & m_window;
};

} // namespace glow
