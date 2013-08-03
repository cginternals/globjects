
#include <cassert>
#include <string>

#include <GL/glew.h>

#ifdef WIN32
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <GL/glx.h>
#include <GL/GLUx.h>
#endif

#include <glow/Window.h>
#include <glow/Log.h>


namespace glow
{

Window::Window()
:   m_fullScreen(false)
{
}

Window::~Window()
{
    if (m_fullScreen)
        toggleFullScreen(false);
}

const bool Window::create(
    const std::string & title
,   const BYTE bgRed
,   const BYTE bgGreen
,   const BYTE bgBlue)
{
    const std::wstring wtitle = std::wstring(title.begin(), title.end());
    static const LPCWSTR className = L"glow::Window";

    const HINSTANCE hInstance(GetModuleHandle(NULL));

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    // register window class

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = Proc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = className;
    wcex.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        fatal() << "Register window class failed (RegisterClassEx). Error: " << GetLastError();
        return false;
    }

    m_hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE
    ,   className
    ,   wtitle.c_str()
    ,   WS_OVERLAPPEDWINDOW
    ,   CW_USEDEFAULT
    ,   CW_USEDEFAULT
    ,   240
    ,   120
    ,   NULL
    ,   NULL
    ,   hInstance
    ,   NULL);

    if (NULL == m_hWnd)
    {
        fatal() << "Create window failed (CreateWindowEx).";
        return false;
    }

    if(ShowWindow(m_hWnd, 0))
        warning() << "Show window failed (ShowWindow).";

    UpdateWindow(m_hWnd);
    return true;
}

void Window::toggleFullScreen(const bool fullScreen)
{
    if (fullScreen == m_fullScreen)
        return;

    if (!fullScreen)
    {
        const LONG result = ChangeDisplaySettings(nullptr, NULL);   // Return to the default mode after a dynamic mode change.
        CheckChangeDisplaySettingsResult(result);
    }
    else
    {
    }
}

void Window::CheckChangeDisplaySettingsResult(const LONG result)
{
    switch (result)
    {
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd183411(v=vs.85).aspx
    case DISP_CHANGE_BADDUALVIEW:
        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADDUALVIEW): The settings change was unsuccessful because the system is DualView capable.";
        break;
    case DISP_CHANGE_BADFLAGS:
        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADFLAGS): An invalid set of flags was passed in.";
        break;
    case DISP_CHANGE_BADMODE:
        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADMODE): The graphics mode is not supported.";
        break;
    case DISP_CHANGE_BADPARAM:
        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADPARAM): An invalid parameter was passed in. This can include an invalid flag or combination of flags.";
        break;
    case DISP_CHANGE_FAILED:
        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_FAILED): The display driver failed the specified graphics mode.";
        break;
    case DISP_CHANGE_NOTUPDATED:
        fatal() << "ChangeDisplaySettings failed(DISP_CHANGE_NOTUPDATED): Unable to write settings to the registry.";
        break;
    case DISP_CHANGE_RESTART:
        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_RESTART): The computer must be restarted for the graphics mode to work.";
        break;

    case DISP_CHANGE_SUCCESSFUL:
        // "DISP_CHANGE_SUCCESSFUL: The settings change was successful."
    default:
        break;
    }
}

LRESULT CALLBACK Window::MessageHandler(
    HWND hWnd
,   UINT message
,   WPARAM wParam
,   LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        DestroyWindow(m_hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK Window::Proc(
    HWND hWnd
,   UINT message
,   WPARAM wParam
,   LPARAM lParam)
{
    Window * pWnd;

    if (message == WM_NCCREATE)
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(LPCREATESTRUCT(lParam)->lpCreateParams));

    pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)); 

    if (pWnd)
        return pWnd->MessageHandler(hWnd, message, wParam, lParam);
    else
        return DefWindowProc(hWnd, message, wParam, lParam);
}

} // namespace glow
