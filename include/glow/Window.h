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

/**

    Attach a WindowEventHandler specialization for event handling.
*/
class GLOW_API Window
{
public:
    Window();
    virtual ~Window();

    bool create(
        const std::string & title = ""
    ,   const unsigned int width = 1280
    ,   const unsigned int height = 720);

    int handle() const;

    void show() const;
    void hide() const;

    void update() const;
    
    void fullScreen();
    void windowed();

    unsigned int width() const;
    unsigned int height() const;

    /** Attaching an eventhandler dispatches an dettach event to the current 
        handler, sets the new handler and dispatches an attach event. The old 
        handler will not be deleted.
    */
    void attachEventHandler(WindowEventHandler * eventHandler);
    WindowEventHandler * eventHandler();

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
    HGLRC m_hRC;
    HDC   m_hDC;

    HWND  m_hWnd;
    RECT  m_rect;

    bool  m_windowed;

    WindowEventHandler * m_eventHandler;
};

} // namespace glow
