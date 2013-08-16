
#ifdef __linux__

#include <cassert>
#include <string>

#include <GL/glx.h>

#include <glow/logging.h>
#include <glow/Screen.h>
#include <glow/Context.h>
#include <glow/WindowEventHandler.h>

#include "X11Window.h"


namespace glow
{

std::set<X11Window*> X11Window::s_windows;

X11Window::X11Window(Window & window)
:   AbstractNativeWindow(window)
,   m_hWnd(0)
,   m_display(nullptr)
{
    s_windows.insert(this);
}

X11Window::~X11Window()
{
    s_windows.erase(s_windows.find(this));
    destroy();
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/dd318252(v=vs.85).aspx

bool X11Window::create(
    const ContextFormat & format
,   const std::string & title
,   const unsigned int width
,   const unsigned int height)
{
    int dummy; // this is stupid! if a parameters is not required passing nullptr does not work.

    m_display = XOpenDisplay(NULL);
    if (NULL == m_display || !glXQueryExtension(m_display, &dummy, &dummy))
    {
        fatal() << "Cannot conntext to X server (XOpenDisplay).";
        return false;
    }

    const int screen = DefaultScreen(m_display);
    const ::Window root = DefaultRootWindow(m_display);

    int attributes[] = { GLX_RGBA, GLX_USE_GL, GLX_DEPTH_SIZE, format.depthBufferSize()
        , (format.swapBehavior() == ContextFormat::DoubleBuffering ? GLX_DOUBLEBUFFER : 0) };

    XVisualInfo * vi = glXChooseVisual(m_display, screen, attributes);
    if (nullptr == vi)
    {
        fatal() << "Choosing a visual failed (glXChooseVisual).";
        return false;
    }

    const Colormap cmap = XCreateColormap(m_display, root, vi->visual, AllocNone);

    XSetWindowAttributes swa;
    swa.colormap     = cmap;
    swa.border_pixel = 0;
    swa.event_mask   = ExposureMask | KeyPressMask | StructureNotifyMask;

    m_hWnd = XCreateWindow(m_display, root, 0, 0, width, height, 0
        , vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
    if (0 == m_hWnd)
    {
        fatal() << "Creating a window failed (XCreateWindow).";
        return false;
    }

    XMapWindow(m_display, m_hWnd);
    XStoreName(m_display, m_hWnd, title.c_str());

    {
        // stupid! see above...
        ::Window dummy2;
        unsigned int dummy3;

        XGetGeometry(m_display, m_hWnd, &dummy2, &m_rect.left, &m_rect.top
            , &m_rect.width, &m_rect.height, &dummy3, &dummy3);
    }
    return true;
}

int X11Window::width() const
{
    return m_rect.width;
}

int X11Window::height() const
{
    return m_rect.height;
}

int X11Window::handle() const
{
    return static_cast<int>(m_hWnd);
}

void X11Window::close()
{
//    SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
}

void X11Window::destroy()
{
//    DestroyWindow(m_hWnd);
}

void X11Window::show()
{
//    ShowWindow(m_hWnd, SW_SHOWNORMAL);
//    UpdateWindow(m_hWnd);
}

void X11Window::hide()
{
//    ShowWindow(m_hWnd, 0);
}

void X11Window::fullScreen()
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

void X11Window::windowed()
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

//void X11Window::restoreDisplaySettings()
//{
//    const LONG result = ChangeDisplaySettings(nullptr, NULL);   // Return to the default mode after a dynamic mode change.
//    if (DISP_CHANGE_SUCCESSFUL == result)
//        return;

//    fatal() << "Toggling to windowed mode failed (ChangeDisplaySettings).";
//    printChangeDisplaySettingsErrorResult(result);
//}

//void X11Window::printChangeDisplaySettingsErrorResult(const LONG result)
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

int X11Window::run()
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

//        for (X11Window * window : s_windows)
//        {
//            msg.hwnd = window->m_hWnd;
//            DispatchMessage(&msg);
//        }

//    } while (WM_QUIT != msg.message);

//    return static_cast<int>(msg.wParam);
    return 0;
}

void X11Window::quit(int code)
{
//    PostQuitMessage(code);
}

void X11Window::repaint()
{
//    InvalidateRect(m_hWnd, NULL, FALSE);
}


void X11Window::onDestroy()
{
//    m_hWnd = 0;

//    if (isFullScreen())
//        restoreDisplaySettings();
}

void X11Window::onRepaint()
{
    if (!context())
        return;

    // http://stackoverflow.com/questions/2842319/swapbuffers-causes-redraw

//    PAINTSTRUCT ps;
//    HDC hDC = BeginPaint(m_hWnd, &ps);

//    AbstractNativeWindow::onRepaint();

//    EndPaint(m_hWnd,&ps);
}


void X11Window::onResize(
    const int width
,   const int height)
{
//    if (width == this->width() && height == this->height())
//        return;

//    GetWindowRect(m_hWnd, &m_rect);

//    AbstractNativeWindow::onResize();
}

//LRESULT CALLBACK X11Window::dispatch(
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

//LRESULT CALLBACK X11Window::InitialProc(
//    HWND hWnd
//,   UINT message
//,   WPARAM wParam
//,   LPARAM lParam)
//{
//    if (message != WM_NCCREATE)
//        return DefWindowProc(hWnd, message, wParam, lParam);

//    LPCREATESTRUCT cstruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
//    void * cparam = cstruct->lpCreateParams;

//    X11Window * window = reinterpret_cast<X11Window*>(cparam);

//    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
//    SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&X11Window::Proc));

//    return window->dispatch(hWnd, message, wParam, lParam);
//}

//LRESULT CALLBACK X11Window::Proc(
//    HWND hWnd
//,   UINT message
//,   WPARAM wParam
//,   LPARAM lParam)
//{
//    LONG_PTR lptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
//    X11Window * window = reinterpret_cast<X11Window*>(lptr);

//    assert(window);

//    return window->dispatch(hWnd, message, wParam, lParam);
//}

} // namespace glow

#endif
