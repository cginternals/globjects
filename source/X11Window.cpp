
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

std::set<X11Window *> X11Window::s_windows;
std::unordered_map< ::Window, X11Window *> X11Window::s_windowsByHandle;

Display * X11Window::s_display = nullptr;

Atom X11Window::s_wmDeleteEvent(-1);
Atom X11Window::s_wmProtocols(-1);

static const Atom IdleEvent = 1;


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

    if(s_windows.empty())
    {
        assert(s_display);

        XCloseDisplay(s_display);
        s_display = nullptr;
    }
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/dd318252(v=vs.85).aspx

Display * X11Window::display()
{
    if(s_display)
        return s_display;

    s_display = XOpenDisplay(NULL);
    if (nullptr == s_display)
    {
        int dummy; // this is stupid! if a parameters is not required passing nullptr does not work.

        if(!glXQueryExtension(s_display, &dummy, &dummy))
        {
            fatal() << "Cannot conntext to X server (XOpenDisplay).";
            return nullptr;
        }
    }

    s_wmProtocols = XInternAtom(s_display, "WM_PROTOCOLS", False);
    s_wmDeleteEvent = XInternAtom(s_display, "WM_DELETE_WINDOW", False);

    return s_display;
}

Bool X11Window::waitForMapNotify(
    Display *
,   XEvent * event
,   char * argument)
{
    return ((MapNotify == event->type) && (reinterpret_cast< ::Window>(argument) == event->xmap.window));
}

void X11Window::backupGeometry()
{
    // awesome dummy derby

    ::Window dummy2;
    unsigned int dummy3;

    XGetGeometry(m_display, m_hWnd, &dummy2, &m_rect.left, &m_rect.top
        , &m_rect.width, &m_rect.height, &dummy3, &dummy3);
}

bool X11Window::create(
    const ContextFormat & format
,   const std::string & title
,   const unsigned int width
,   const unsigned int height)
{
    m_display = display();
    if (nullptr == m_display)
        return false;

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
    s_windowsByHandle[m_hWnd] = this;

    // setup additional events
    XSetWMProtocols(s_display, m_hWnd, &s_wmDeleteEvent, 1);

    XStoreName(m_display, m_hWnd, title.c_str());

    backupGeometry();

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
    // http://john.nachtimwald.com/2009/11/08/sending-wm_delete_window-client-messages/

    XEvent event;
    memset(&event, 0, sizeof(event));

    event.type = ClientMessage;

    event.xclient.window = m_hWnd;
    event.xclient.message_type = s_wmProtocols;
    event.xclient.format = 32;
    event.xclient.data.l[0] = s_wmDeleteEvent;
    event.xclient.data.l[1] = CurrentTime;

    XSendEvent(s_display, m_hWnd, False, NoEventMask, &event);
}

void X11Window::show()
{
    XMapWindow(m_display, m_hWnd);

    XEvent event;
    XIfEvent(m_display, &event, waitForMapNotify, reinterpret_cast<char*>(m_hWnd));
}

void X11Window::hide()
{
    XUnmapWindow(m_display, m_hWnd);

    XEvent event;
    XIfEvent(m_display, &event, waitForMapNotify, reinterpret_cast<char*>(m_hWnd));
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

int X11Window::run()
{
    if(nullptr == display())
        return false;

    XEvent event;
    do
    {
        if (XPending(s_display))
        {
            XNextEvent(s_display, &event);
            dispatchEvent(event);
            continue;
        }

        event.type = ClientMessage;
        for (X11Window * window : s_windows)
        {
            event.xclient.display = s_display;
            event.xclient.window = window->m_hWnd;
            event.xclient.message_type = IdleEvent;
            event.xclient.format = 8; // NOTE: format is mandatory and should be set.

            XSendEvent(s_display, window->m_hWnd, False, NoEventMask, &event);
        }

    } while (true); //WM_QUIT != event.type);

//    return static_cast<int>(msg.wParam);
    return 0;
}

void X11Window::quit(int code)
{
    exit(code);
}

void X11Window::repaint()
{
    XEvent event;
    memset(&event, 0, sizeof(event));

    event.type = Expose;

    event.xexpose.window = m_hWnd;
    event.xexpose.display = s_display;

    event.xexpose.x = m_rect.left;
    event.xexpose.y = m_rect.top;
    event.xexpose.width  = m_rect.width;
    event.xexpose.height = m_rect.height;

    event.xexpose.count = 1;

    XSendEvent(s_display, m_hWnd, False, NoEventMask, &event);
}


void X11Window::onDestroy()
{
    warning() << "destroy";
    s_windowsByHandle.erase(m_hWnd);

    XDestroyWindow(m_display, m_hWnd);
    m_hWnd = 0;

//    if (isFullScreen())
//        restoreDisplaySettings();
}

void X11Window::onRepaint()
{
    if (!context())
        return;

    AbstractNativeWindow::onRepaint();
}


void X11Window::onResize(
    const int width
,   const int height)
{
    if (width == this->width() && height == this->height())
        return;

    backupGeometry();

    AbstractNativeWindow::onResize();
}

int X11Window::dispatch(
    const ::Window hWnd
,   const XEvent & event)
{
    assert(!m_hWnd || hWnd == m_hWnd);

    if (!m_hWnd)
        return 0;


    switch (event.type)
    {
    case ConfigureNotify:
        onResize(event.xconfigure.width, event.xconfigure.height);
        break;

    case DestroyNotify:
        warning() << "destroy notify";
        onDestroy();
        break;

    case Expose:
        onRepaint();
        break;

    case ClientMessage:
        if (event.xclient.message_type == IdleEvent)
            onIdle();
        else if (s_wmDeleteEvent == event.xclient.data.l[0])
            onClose();
        break;

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

    default:
        return 0;
    }
    return 0;
}

int X11Window::dispatchEvent(const XEvent & event)
{
    const auto f = s_windowsByHandle.find(event.xany.window);
    if(f == s_windowsByHandle.cend())
        return 0;

    X11Window * window(f->second);
    return window->dispatch(window->m_hWnd, event);
}

} // namespace glow

#endif
