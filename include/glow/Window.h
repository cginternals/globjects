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
    ,   const BYTE bgRed   = 1.f
    ,   const BYTE bgGreen = 1.f
    ,   const BYTE bgBlue  = 1.f);

    void toggleFullScreen(const bool fullScreen);


    // design similar to http://www.codeproject.com/Articles/2556/A-Simple-Win32-Window-Wrapper-Class

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

    static void CheckChangeDisplaySettingsResult(const LONG result);

protected:
    HGLRC   m_hRC;
    HDC     m_hDC;

    HWND    m_hWnd;

    bool    m_fullScreen;
};

} // namespace glow
