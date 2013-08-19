#pragma once

#ifdef __linux__

#include <set>
#include <unordered_map>
#include <string>
#include <X11/X.h>
#include <X11/Xlib.h>

#include <glow/glow.h>
#include <glow/KeyEvent.h>

#include "AbstractNativeWindow.h"


namespace glow
{

class WindowEventHandler;
class ContextFormat;
class Context;

class X11Window : public AbstractNativeWindow
{
    struct Rect
    {
        int left;
        int top;
        unsigned int width;
        unsigned int height;
    };

public:
    X11Window(Window & window);
    virtual ~X11Window();

    virtual bool create(
        const ContextFormat & format
    ,   const std::string & title
    ,   const unsigned int width
    ,   const unsigned int height);

    virtual void close();
    virtual void destroy();

    virtual int width() const;
    virtual int height() const;

    virtual int handle() const;

    virtual void repaint();

    virtual void show();
    virtual void hide();

    virtual void fullScreen();
    virtual void windowed();

public:
    static int run();
    static void quit(int code = 0);

protected:

    // design similar to:
    // http://www.codeproject.com/Articles/2556/A-Simple-Win32-Window-Wrapper-Class
    // http://members.gamedev.net/sicrane/articles/WindowClass.html

    static Bool waitForMapNotify(
        Display * display
    ,   XEvent * event
    ,   char * argument);

    static int dispatchEvent(const XEvent & event);

    static Display * display();

//    static void restoreDisplaySettings();
//    static void printChangeDisplaySettingsErrorResult(const LONG result);

protected:
    int dispatch(
        const ::Window hWnd   ///< Checked against own handle.
    ,   const XEvent & event);

protected:
    static std::set<X11Window*> s_windows;
    static std::unordered_map< ::Window, X11Window *> s_windowsByHandle;

    static Display * s_display;

protected:
    void onRepaint();
    void onResize(
        const int width
    ,   const int height);

    void onDestroy();

protected:
    Display * m_display;
    ::Window m_hWnd;

    Rect m_rect;
    Rect m_backup;
};

} // namespace glow

#endif
