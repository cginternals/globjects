
#include <cassert>
#include <string>

#ifdef WIN32
#include <Windows.h>
#else
#endif

#include <glow/Screen.h>
#include <glow/Log.h>
#include <glow/WindowEventHandler.h>

#include <glow/Window.h>

namespace glow
{

Window::Window()
:   m_hWnd(0)
,   m_hDC(0)
,   m_hRC(0)

,   m_windowed(true)

,   m_eventHandler(nullptr)
{
}

Window::~Window()
{
    if (!m_windowed)
        restoreDisplaySettings();
}

bool Window::create(
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
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = className;
    wcex.hIconSm        = NULL;

    if (!RegisterClassEx(&wcex))
    {
        fatal() << "Registering the window class failed (RegisterClassEx). Error: " << GetLastError();
        return false;
    }

    m_hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE
    ,   className
    ,   wtitle.c_str()
    ,   WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
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

int Window::handle() const
{
    return reinterpret_cast<int>(m_hWnd);
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

    // check current desktop size

    unsigned int screenWidth;
    unsigned int screenHeight;

    Screen::getDesktopResolution(screenWidth, screenHeight);

    // only change display settings on different resolutions.

    if (width() != screenWidth || height() != screenHeight)
    {
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
            printChangeDisplaySettingsErrorResult(result);

            return;
        }
    }

    // move window to screen origin

    RECT rect;
    rect.left   = 0L;
    rect.right  = width();
    rect.top    = 0L;
    rect.bottom = height();

    SetWindowLongPtr(m_hWnd, GWL_STYLE
        , WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
    MoveWindow(m_hWnd, 0, 0, width(), height(), TRUE);

    SetForegroundWindow(m_hWnd);
    SetFocus(m_hWnd);

    update();
}

void Window::windowed()
{
    if (m_windowed)
        return;

    m_windowed = true;

    restoreDisplaySettings();

    SetWindowLongPtr(m_hWnd, GWL_STYLE
        , WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    // NOTE: do not use AdjustWindowRect.. 
    // It somehow messes up the position restore (client origin instead of 
    // window origin). Working solution here: 

    // http://stackoverflow.com/questions/7193197/is-there-a-graceful-way-to-handle-toggling-between-fullscreen-and-windowed-mode

    MoveWindow(m_hWnd, m_rect.left, m_rect.top, width(), height(), TRUE);

    update();
}

unsigned int Window::width() const
{
    return m_rect.right - m_rect.left;
}

unsigned int Window::height() const
{
    return m_rect.bottom- m_rect.top;
}

void Window::restoreDisplaySettings()
{
    const LONG result = ChangeDisplaySettings(nullptr, NULL);   // Return to the default mode after a dynamic mode change.
    if (DISP_CHANGE_SUCCESSFUL == result)
        return;

    fatal() << "Toggling to windowed mode failed (ChangeDisplaySettings).";
    printChangeDisplaySettingsErrorResult(result);
}

void Window::printChangeDisplaySettingsErrorResult(const LONG result)
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

void Window::attachEventHandler(WindowEventHandler * eventHandler)
{
    if (m_eventHandler == eventHandler)
        return;

    if (m_eventHandler)
        m_eventHandler->dettachEvent(this);

    m_eventHandler = eventHandler;

    if (m_eventHandler)
        m_eventHandler->attachEvent(this);
}

WindowEventHandler * Window::eventHandler()
{
    return m_eventHandler;
}

LRESULT CALLBACK Window::dispatch(
    HWND hWnd
,   UINT message
,   WPARAM wParam
,   LPARAM lParam)
{
    if (!m_eventHandler)
        return DefWindowProc(hWnd, message, wParam, lParam);

    // Windows Messages: http://msdn.microsoft.com/en-us/library/windows/desktop/ms644927(v=vs.85).aspx#windows_messages
    switch (message)
    {
    case WM_CLOSE:
        m_eventHandler->closeEvent();
        DestroyWindow(m_hWnd);
        break;

    case WM_DESTROY:
        m_eventHandler->destroyEvent();
        break;

    case WM_SIZING:
        GetWindowRect(m_hWnd, &m_rect);
        m_eventHandler->resizeEvent(width(), height());
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

    return window->dispatch(hWnd, message, wParam, lParam);
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

    return window->dispatch(hWnd, message, wParam, lParam);
}

} // namespace glow
