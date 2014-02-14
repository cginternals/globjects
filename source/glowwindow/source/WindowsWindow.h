#pragma once

#ifdef WIN32

#include <set>
#include <string>
#include <windows.h>

#include <glow/glow.h>
#include <glowwindow/KeyEvent.h>

#include "AbstractNativeWindow.h"


namespace glow 
{

class WindowEventHandler;
class ContextFormat;
class Context;

class WindowsWindow : public AbstractNativeWindow
{
public:
    WindowsWindow(Window & window);
    virtual ~WindowsWindow();

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

    virtual void setTitle(const std::string & title);

public:
    static int run();
    static void quit(const int code = 0);

protected:

    // design similar to:
    // http://www.codeproject.com/Articles/2556/A-Simple-Win32-Window-Wrapper-Class
    // http://members.gamedev.net/sicrane/articles/WindowClass.html

    static LRESULT CALLBACK InitialProc(
        HWND hWnd
    ,   UINT message
    ,   WPARAM wParam
    ,   LPARAM lParam);

    static LRESULT CALLBACK Proc(
        HWND hWnd   ///< Unique handle of the window.
    ,   UINT message
    ,   WPARAM wParam
    ,   LPARAM lParam);

    LRESULT CALLBACK dispatch(
        const HWND hWnd   ///< Checked against own handle.
    ,   const UINT message
    ,   const WPARAM wParam
    ,   const LPARAM lParam);

    static void restoreDisplaySettings();
    static void printChangeDisplaySettingsErrorResult(const LONG result);

protected:
    static std::set<WindowsWindow*> s_windows;

protected:
    void onRepaint();
    void onResize(
        const int width
    ,   const int height);

    void onDestroy();

protected:
    HWND  m_hWnd;

    RECT m_rect;
    RECT m_backup;
};

} // namespace glow

#endif
