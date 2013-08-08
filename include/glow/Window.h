#pragma once

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

/**

    Attach a WindowEventHandler specialization for event handling.
*/
class GLOW_API Window
{
public:
    Window();
    virtual ~Window();

    bool create(
        const ContextFormat & format
    ,   const std::string & title = ""
    ,   const unsigned int width = 1280
    ,   const unsigned int height = 720);

    int handle() const;

    void show() const;
    void hide() const;

    void update() const;

    /** This enters the (main) windows message loop and dispatches events to
        the attached WindowEventHandler instance.
    */
    int run(
        WindowEventHandler * eventHandler = nullptr
    ,   const unsigned int paintInterval = 0);

    void paint();

    int width() const;
    int height() const;

    void fullScreen();
    void windowed();

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

protected:

    LRESULT CALLBACK dispatch(
        HWND hWnd
    ,   UINT message
    ,   WPARAM wParam
    ,   LPARAM lParam);

    static void printChangeDisplaySettingsErrorResult(const LONG result);
    static void restoreDisplaySettings();

protected:
    HWND  m_hWnd;

    int m_left;
    int m_top;

    int m_width;
    int m_height;

    bool  m_windowed;

    WindowEventHandler * m_eventHandler;
    Context * m_context;
};

} // namespace glow
