
#ifdef __linux__

#include <cassert>
#include <string>

#include <glow/logging.h>
#include <glow/Screen.h>
#include <glow/Context.h>
#include <glow/WindowEventHandler.h>

#include "LinWindow.h"


namespace glow
{

std::set<LinWindow*> LinWindow::s_windows;

LinWindow::LinWindow(Window & window)
:   AbstractNativeWindow(window)
,   m_hWnd(0)
,   m_display(nullptr)
{
    s_windows.insert(this);
}

LinWindow::~LinWindow()
{
    s_windows.erase(s_windows.find(this));
    destroy();
}

bool LinWindow::create(
    const ContextFormat & format
,   const std::string & title
,   const unsigned int width
,   const unsigned int height)
{
    m_display = XOpenDisplay(NULL);
    if (NULL == m_display)
    {
        fatal() << "Cannot conntext to X server (XOpenDisplay).";
        return false;
    }

    ::Window root = DefaultRootWindow(m_display);

    m_hWnd = XCreateWindow(m_display, root, 0, 0, width, height, 0
        , CopyFromParent, CopyFromParent, CopyFromParent, 0, 0);

    if (0 == m_hWnd)
    {
        fatal() << "Creating a window failed (XCreateWindow).";
        return false;
    }

    XMapWindow(m_display, m_hWnd);
    XStoreName(m_display, m_hWnd, title.c_str());

    {
        // NOTE: this is a stupid api! if a parameters is not required passing nullptr does not work.

        ::Window dummy0;

        unsigned int dummy1;
        unsigned int dummy2;

        XGetGeometry(m_display, m_hWnd, &dummy0, &m_rect.left, &m_rect.top
            , &m_rect.width, &m_rect.height, &dummy1, &dummy2);
    }

    return true;
}

int LinWindow::width() const
{
    return m_rect.width;
}

int LinWindow::height() const
{
    return m_rect.height;
}

int LinWindow::handle() const
{
    return static_cast<int>(m_hWnd);
}

void LinWindow::close()
{
//    SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
}

void LinWindow::destroy()
{
//    DestroyWindow(m_hWnd);
}

void LinWindow::show()
{
//    ShowWindow(m_hWnd, SW_SHOWNORMAL);
//    UpdateWindow(m_hWnd);
}

void LinWindow::hide()
{
//    ShowWindow(m_hWnd, 0);
}

void LinWindow::fullScreen()
{
//    GetWindowRect(m_hWnd, &m_backup);

//    SetWindowLongPtr(m_hWnd, GWL_STYLE
//        , WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

//    // check current desktop size

//    unsigned int screenWidth;
//    unsigned int screenHeight;

//    Screen::getDesktopResolution(screenWidth, screenHeight);

//    // only change display settings on different resolutions.

//    if (width() != screenWidth || height() != screenHeight)
//    {
//        // TODO: retrieve nearest supported resolution

//        DEVMODE dm;
//        ZeroMemory(&dm, sizeof(DEVMODE));

//        dm.dmSize = sizeof(DEVMODE);

//        dm.dmPelsWidth  = width();
//        dm.dmPelsHeight = height();
//        dm.dmBitsPerPel = 32;

//        dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

//        const LONG result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
//        if (DISP_CHANGE_SUCCESSFUL != result)
//        {
//            fatal() << "Toggling to fullscreen mode failed (ChangeDisplaySettings).";
//            printChangeDisplaySettingsErrorResult(result);

//            return;
//        }
//    }
//    MoveWindow(m_hWnd, 0, 0, width(), height(), TRUE);

//    SetForegroundWindow(m_hWnd);
//    SetFocus(m_hWnd);
}

void LinWindow::windowed()
{
//    // http://stackoverflow.com/questions/7193197/is-there-a-graceful-way-to-handle-toggling-between-fullscreen-and-windowed-mode

//    restoreDisplaySettings();

//    // NOTE: do not use AdjustWindowRect..
//    // It somehow messes up the position restore (client origin instead of
//    // window origin). Working solution here:

//    int w = m_backup.right - m_backup.left;
//    int h = m_backup.bottom - m_backup.top;

//    MoveWindow(m_hWnd, m_backup.left, m_backup.top, w, h, TRUE);

//    SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
}

//void LinWindow::restoreDisplaySettings()
//{
//    const LONG result = ChangeDisplaySettings(nullptr, NULL);   // Return to the default mode after a dynamic mode change.
//    if (DISP_CHANGE_SUCCESSFUL == result)
//        return;

//    fatal() << "Toggling to windowed mode failed (ChangeDisplaySettings).";
//    printChangeDisplaySettingsErrorResult(result);
//}

//void LinWindow::printChangeDisplaySettingsErrorResult(const LONG result)
//{
//    switch (result)
//    {
//    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd183411(v=vs.85).aspx
//    case DISP_CHANGE_BADDUALVIEW:
//        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADDUALVIEW): The settings change was unsuccessful because the system is DualView capable.";
//        break;
//    case DISP_CHANGE_BADFLAGS:
//        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADFLAGS): An invalid set of flags was passed in.";
//        break;
//    case DISP_CHANGE_BADMODE:
//        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADMODE): The graphics mode is not supported.";
//        break;
//    case DISP_CHANGE_BADPARAM:
//        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_BADPARAM): An invalid parameter was passed in. This can include an invalid flag or combination of flags.";
//        break;
//    case DISP_CHANGE_FAILED:
//        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_FAILED): The display driver failed the specified graphics mode.";
//        break;
//    case DISP_CHANGE_NOTUPDATED:
//        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_NOTUPDATED): Unable to write settings to the registry.";
//        break;
//    case DISP_CHANGE_RESTART:
//        fatal() << "ChangeDisplaySettings failed (DISP_CHANGE_RESTART): The computer must be restarted for the graphics mode to work.";
//        break;

//    case DISP_CHANGE_SUCCESSFUL:
//        // "DISP_CHANGE_SUCCESSFUL: The settings change was successful."
//    default:
//        break;
//    }
//}

int LinWindow::run()
{
//    MSG  msg;
//    do
//    {
//        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);

//            continue;
//        }

//        msg.message = WM_USER_IDLE;

//        for (LinWindow * window : s_windows)
//        {
//            msg.hwnd = window->m_hWnd;
//            DispatchMessage(&msg);
//        }

//    } while (WM_QUIT != msg.message);

//    return static_cast<int>(msg.wParam);
    return 0;
}

void LinWindow::quit(int code)
{
//    PostQuitMessage(code);
}

void LinWindow::repaint()
{
//    InvalidateRect(m_hWnd, NULL, FALSE);
}


void LinWindow::onDestroy()
{
//    m_hWnd = 0;

//    if (isFullScreen())
//        restoreDisplaySettings();
}

void LinWindow::onRepaint()
{
    if (!context())
        return;

    // http://stackoverflow.com/questions/2842319/swapbuffers-causes-redraw

//    PAINTSTRUCT ps;
//    HDC hDC = BeginPaint(m_hWnd, &ps);

//    AbstractNativeWindow::onRepaint();

//    EndPaint(m_hWnd,&ps);
}


void LinWindow::onResize(
    const int width
,   const int height)
{
//    if (width == this->width() && height == this->height())
//        return;

//    GetWindowRect(m_hWnd, &m_rect);

//    AbstractNativeWindow::onResize();
}

//LRESULT CALLBACK LinWindow::dispatch(
//    HWND hWnd
//,   UINT message
//,   WPARAM wParam
//,   LPARAM lParam)
//{
//    assert(!m_hWnd || hWnd == m_hWnd);

//    if (!m_hWnd)
//        return DefWindowProc(hWnd, message, wParam, lParam);

//    // Windows Messages: http://msdn.microsoft.com/en-us/library/windows/desktop/ms644927(v=vs.85).aspx#windows_messages
//    switch (message)
//    {
//    case WM_CLOSE:
//        onClose();
//        break;

//    case WM_DESTROY:
//        onDestroy();
//        break;

//    case WM_SIZE:
//        onResize(LOWORD(lParam), HIWORD(lParam));
//        break;

//    case WM_PAINT:
//        onRepaint();
//        break;

//    case WM_USER_IDLE:
//        onIdle();
//        break;

//    case WM_KEYDOWN:
//    case WM_SYSKEYDOWN:
//        if(!onKeyPress(LOWORD(wParam)))
//            DefWindowProc(hWnd, message, wParam, lParam);
//        break;

//    case WM_KEYUP:
//    case WM_SYSKEYUP:
//        if(!onKeyRelease(LOWORD(wParam)))
//            DefWindowProc(hWnd, message, wParam, lParam);
//        break;

//    //case WM_ACTIVATE:
//    //    // Check for minimization.
//    //    if (!HIWORD(wParam))
//    //        m_eventHandler->activateEvent();
//    //    else
//    //        m_eventHandler->minimizeEvent();
//    //    break;

//    case WM_SYSCOMMAND:
//        // Absorb some system commands.
//        switch (wParam)
//        {
//        case SC_SCREENSAVE:
//        case SC_MONITORPOWER:
//            return 0;
//        }

//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}

//LRESULT CALLBACK LinWindow::InitialProc(
//    HWND hWnd
//,   UINT message
//,   WPARAM wParam
//,   LPARAM lParam)
//{
//    if (message != WM_NCCREATE)
//        return DefWindowProc(hWnd, message, wParam, lParam);

//    LPCREATESTRUCT cstruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
//    void * cparam = cstruct->lpCreateParams;

//    LinWindow * window = reinterpret_cast<LinWindow*>(cparam);

//    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
//    SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&LinWindow::Proc));

//    return window->dispatch(hWnd, message, wParam, lParam);
//}

//LRESULT CALLBACK LinWindow::Proc(
//    HWND hWnd
//,   UINT message
//,   WPARAM wParam
//,   LPARAM lParam)
//{
//    LONG_PTR lptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
//    LinWindow * window = reinterpret_cast<LinWindow*>(lptr);

//    assert(window);

//    return window->dispatch(hWnd, message, wParam, lParam);
//}

} // namespace glow

#endif
