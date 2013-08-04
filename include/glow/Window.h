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

class GLOW_API Window
{
public:
    Window();
    virtual ~Window();

    const bool create(
        const std::string & title = ""
    ,   const unsigned int width = 1280
    ,   const unsigned int height = 720);

    /** Shows and updates the window 
    */
    void show() const;

    void update() const;
    
    /** Hides the window. 
    */
    void hide() const;

    void fullScreen();
    void windowed();

    const unsigned int width() const;
    const unsigned int height() const;

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

    LRESULT CALLBACK MessageHandler(
        HWND hWnd       ///< Unique handle of the window. Check this against own handle.
    ,   UINT message
    ,   WPARAM wParam
    ,   LPARAM lParam);

    static void PrintChangeDisplaySettingsErrorResult(const LONG result);

protected:
    HGLRC m_hRC;
    HDC   m_hDC;

    HWND  m_hWnd;
    RECT  m_rect;


    bool  m_windowed;

};

} // namespace glow