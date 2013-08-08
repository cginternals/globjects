#pragma once

#include <set>

// TODO: remove 
#ifdef WIN32
#include <windows.h>
#endif

#include <glow/glow.h>


namespace std
{
    string;
}

namespace glow 
{

class WindowEventHandler;
class ContextFormat;
class Context;
class KeyEvent;

/**

    Attach a WindowEventHandler specialization for event handling.
*/
class GLOW_API Window
{
public:
    Window();
    virtual ~Window();

    void attach(WindowEventHandler * eventHandler);

    bool create(
        const ContextFormat & format
    ,   const std::string & title = ""
    ,   const unsigned int width = 1280
    ,   const unsigned int height = 720);

    void destroy();

    int handle() const;
    Context * context();

    void repaint();

    void show() const;
    void hide() const;

    int width() const;
    int height() const;

    void fullScreen();
    void windowed();
    
    /** If enabled, this causes an application wide quit message to be posted
        when the window gets destroyed. Hence, the static window loop (run) 
        will receive a quit event and destroy all other remaining windows.
    */
    void setQuitOnDestroy(const bool enable);

    // design similar to:
    // http://www.codeproject.com/Articles/2556/A-Simple-Win32-Window-Wrapper-Class
    // http://members.gamedev.net/sicrane/articles/WindowClass.html

    static LRESULT CALLBACK InitialProc(
        HWND hWnd
    ,   UINT message
    ,   WPARAM wParam
    ,   LPARAM lParam);

    static LRESULT CALLBACK Proc(
        HWND hWnd       ///< Unique handle of the window. Check this against own handle.
    ,   UINT message
    ,   WPARAM wParam
    ,   LPARAM lParam);

public:
    
    /** This enters the (main) windows message loop and dispatches events to
        the attached WindowEventHandler instance.
    */
    static int run();
    static void quit(const int code = 0);

protected:
    void onResize(
        const int width
    ,   const int height);

    void onRepaint();
    void onIdle();

    void onClose();
    void onDestroy();

    void onValidContext();

protected:
    LRESULT CALLBACK dispatch(
        HWND hWnd
    ,   UINT message
    ,   WPARAM wParam
    ,   LPARAM lParam);

    static void restoreDisplaySettings();

    void processKeyEvent(
        KeyEvent & event
    ,   WindowEventHandler * eventHandler);

protected:
    HWND  m_hWnd;

    WindowEventHandler * m_eventHandler;
    Context * m_context;

    bool m_windowed;
    bool m_quitOnDestroy;

    int m_left;
    int m_top;

    int m_width;
    int m_height;

protected:
    static std::set<Window*> s_windows;
};

} // namespace glow
