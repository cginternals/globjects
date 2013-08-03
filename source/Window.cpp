
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
:   m_hWnd(0)
,   m_hDC(0)
,   m_hRC(0)

,   m_windowed(true)
{
}

Window::~Window()
{
    if (!m_windowed)
        windowed();
}

const bool Window::create(
    const std::string & title
,   const unsigned int width
,   const unsigned int height)
{
    const std::wstring wtitle = std::wstring(title.begin(), title.end());
    static const LPCWSTR className = L"glow::Window";

    const HINSTANCE hInstance(GetModuleHandle(NULL));

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    // register window class

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc    = InitialProc;
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
        fatal() << "Registering the window class failed (RegisterClassEx). Error: " << GetLastError();
        return false;
    }

    m_hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE
    ,   className
    ,   wtitle.c_str()
    ,   WS_OVERLAPPEDWINDOW
    ,   CW_USEDEFAULT
    ,   CW_USEDEFAULT
    ,   width
    ,   height
    ,   NULL
    ,   NULL
    ,   hInstance
    ,   this);

    if (NULL == m_hWnd)
    {
        fatal() << "Creating a window failed (CreateWindowEx). Error: " << GetLastError();
        return false;
    }

    update();

    GetWindowRect(m_hWnd, &m_rect);

    assert(this->width()  == width);
    assert(this->height() == height);

    return true;
}

void Window::show() const
{
    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    update();
}

void Window::update() const
{
    UpdateWindow(m_hWnd);
}

void Window::hide() const
{
    ShowWindow(m_hWnd, 0);
}

void Window::fullScreen()
{
    if (!m_windowed)
        return;

    m_windowed = false;

    DEVMODE dm;
    ZeroMemory(&dm, sizeof(DEVMODE));

    dm.dmSize = sizeof(DEVMODE);

    dm.dmPelsWidth  = width();
    dm.dmPelsHeight = height();
    dm.dmBitsPerPel = 32;

    dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    const LONG result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
    if (DISP_CHANGE_SUCCESSFUL != result)
    {
        fatal() << "Toggling to fullscreen mode failed (ChangeDisplaySettings).";
        PrintChangeDisplaySettingsErrorResult(result);

        return;
    }

    RECT rect;
    rect.left   = 0L;
    rect.right  = width();
    rect.top    = 0L;
    rect.bottom = height();

    if (0 == AdjustWindowRectEx(&rect, WS_POPUP, NULL, WS_EX_APPWINDOW))
        fatal() << "Adjusting window rect failed (AdjustWindowRectEx).";

//    SetWindowPos(m_hWnd, HWND_TOP, rect.left, rect.top, 0, 0, SWP_NOSIZE);

    update();
}

void Window::windowed()
{
    if (m_windowed)
        return;

    m_windowed = true;

    const LONG result = ChangeDisplaySettings(nullptr, NULL);   // Return to the default mode after a dynamic mode change.
    if (DISP_CHANGE_SUCCESSFUL != result)
    {
        fatal() << "Toggling to windowed mode failed (ChangeDisplaySettings).";
        PrintChangeDisplaySettingsErrorResult(result);

        return;
    }

    AdjustWindowRectEx(&m_rect, WS_OVERLAPPEDWINDOW, NULL, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
    update();
}

const unsigned int Window::width() const
{
    return m_rect.right - m_rect.left;
}

const unsigned int Window::height() const
{
    return m_rect.bottom- m_rect.top;
}

void Window::PrintChangeDisplaySettingsErrorResult(const LONG result)
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

LRESULT CALLBACK Window::InitialProc(
    HWND hWnd
,   UINT message
,   WPARAM wParam
,   LPARAM lParam)
{
    if (message != WM_NCCREATE)
        return DefWindowProc(hWnd, message, wParam, lParam);

    LPCREATESTRUCT cstruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
    void * cparam = cstruct->lpCreateParams;
        
    Window * window = reinterpret_cast<Window*>(cparam);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::Proc));

    return window->MessageHandler(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK Window::Proc(
    HWND hWnd
,   UINT message
,   WPARAM wParam
,   LPARAM lParam)
{
    LONG_PTR lptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    Window * window = reinterpret_cast<Window*>(lptr); 

    assert(window);

    return window->MessageHandler(hWnd, message, wParam, lParam);
}

} // namespace glow
