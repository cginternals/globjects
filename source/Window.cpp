
#include <cassert>
#include <string>

#ifdef WIN32
#include <Windows.h>
#else
#endif

#include <glow/Screen.h>
#include <glow/Log.h>
#include <glow/WindowEventHandler.h>
#include <glow/Context.h>
#include <glow/KeyEvent.h>

#include <glow/Window.h>

namespace glow
{

namespace
{
#define WM_USER_IDLE            WM_USER + 1
#define WM_USER_VALID_CONTEXT   WM_USER + 2
}

std::set<Window*> Window::s_windows;

/** NOTE: These static inline functions are not part of the window interface,
    to provide identical interfaces over all supported platforms.
*/

static inline void printChangeDisplaySettingsErrorResult(const LONG result)
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

Window::Rect::Rect(
    int left
,   int top
,   int width
,   int height)
:   left(left)
,   top(top)
,   width(width)
,   height(height)
{
}

Window::Window()
:   m_hWnd(0)

,   m_eventHandler(nullptr)
,   m_context(nullptr)

,   m_mode(WindowMode)
,   m_quitOnDestroy(true)
{
    s_windows.insert(this);
}

Window::~Window()
{
    destroy();

    s_windows.erase(s_windows.find(this));
}

void Window::setQuitOnDestroy(const bool enable)
{
    m_quitOnDestroy = enable;
}

bool Window::create(
    const ContextFormat & format
,   const std::string & title
,   const unsigned int width
,   const unsigned int height)
{
    const std::wstring wtitle = std::wstring(title.begin(), title.end());
    static const LPCWSTR className = L"glow::Window";

    const HINSTANCE hInstance(GetModuleHandle(NULL));

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    if (FALSE == GetClassInfoEx(hInstance, className, &wcex))
    {
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

    UpdateWindow(m_hWnd);

    // retrieve window rect

    RECT rect;
    GetWindowRect(m_hWnd, &rect);

    m_rect = Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

    assert(m_rect.width  == width);
    assert(m_rect.height == height);

    // setup context

    m_context = new Context();
    const bool result = m_context->create(handle(), format);

    SendMessage(m_hWnd, WM_USER_VALID_CONTEXT, NULL, NULL);

    return result;
}

void Window::onClose()
{
    destroy();
}

void Window::destroy()
{
    if (m_context)
    {
        if (m_eventHandler)
        {
            m_context->makeCurrent();
            m_eventHandler->deinitializeEvent(*this);
            m_context->doneCurrent();
        }

        m_context->release();
        delete m_context;

        m_context = nullptr;
    }

    DestroyWindow(m_hWnd);

    if (m_quitOnDestroy)
        quit(0);
}

void Window::onDestroy()
{
    m_hWnd = 0;

    if (m_mode != WindowMode)
        restoreDisplaySettings();
}

int Window::handle() const
{
    return reinterpret_cast<int>(m_hWnd);
}

Context * Window::context()
{
    return m_context;
}

void Window::show() const
{
    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);
}

void Window::hide() const
{
    ShowWindow(m_hWnd, 0);
}

void Window::attach(WindowEventHandler * eventHandler)
{
    m_eventHandler = eventHandler;

    if (!m_eventHandler)
        return;

    if (!m_context)
        return;

    warning() << "Consider attaching window event handler earlier.";

    assert(m_hWnd);
    SendMessage(m_hWnd, WM_USER_VALID_CONTEXT, NULL, NULL);
}

int Window::run()
{
    MSG  msg;
    do
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            continue;
        }

        msg.message = WM_USER_IDLE;
        for (Window * window : s_windows)
        {
            msg.hwnd = reinterpret_cast<HWND>(window->handle());
            DispatchMessage(&msg);
        }

    } while (WM_QUIT != msg.message);

    return static_cast<int>(msg.wParam);
}

void Window::quit(const int code)
{
    PostQuitMessage(code);
}

void Window::repaint()
{
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void Window::onRepaint()
{
    if (!m_context)
        return;

    // http://stackoverflow.com/questions/2842319/swapbuffers-causes-redraw

    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(m_hWnd, &ps);

    m_context->makeCurrent();

    if (m_eventHandler)
        m_eventHandler->paintEvent(*this);

    m_context->swap();
    m_context->doneCurrent();

    EndPaint(m_hWnd,&ps);
}

void Window::onValidContext()
{
    if(!m_context || !m_eventHandler)
        return;

    m_context->makeCurrent();

    m_eventHandler->initializeEvent(*this);
    m_eventHandler->resizeEvent(*this, m_rect.width, m_rect.height);

    m_context->doneCurrent();
}

void Window::onResize(
    const int width
,   const int height)
{
    if (width == m_rect.width && height == m_rect.height)
        return;

    m_rect.width  = width;
    m_rect.height = height;

    if (m_context && m_eventHandler)
    {
        m_context->makeCurrent();
        m_eventHandler->resizeEvent(*this, m_rect.width, m_rect.height);
        m_context->doneCurrent();
    }
}

void Window::onIdle()
{
    if (m_eventHandler)
        m_eventHandler->idleEvent(*this);
}

void Window::fullScreen()
{
    if (WindowMode != m_mode)
        return;

    m_mode = TransitionMode;

    // backup window rect

    RECT rect;
    GetWindowRect(m_hWnd, &rect);

    m_backup = Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

    SetWindowLongPtr(m_hWnd, GWL_STYLE
        , WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

    // check current desktop size

    unsigned int screenWidth;
    unsigned int screenHeight;

    Screen::getDesktopResolution(screenWidth, screenHeight);


    // only change display settings on different resolutions.

    if (m_rect.width != screenWidth || m_rect.height != screenHeight)
    {
        // TODO: retrieve nearest supported resolution

        DEVMODE dm;
        ZeroMemory(&dm, sizeof(DEVMODE));

        dm.dmSize = sizeof(DEVMODE);

        dm.dmPelsWidth  = m_rect.width;
        dm.dmPelsHeight = m_rect.height;
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

    MoveWindow(m_hWnd, 0, 0, m_rect.width, m_rect.height, TRUE);

    SetForegroundWindow(m_hWnd);
    SetFocus(m_hWnd);

    m_mode = FullScreenMode;
}

void Window::windowed()
{
    if (FullScreenMode != m_mode)
        return;

    m_mode = TransitionMode;

    // http://stackoverflow.com/questions/7193197/is-there-a-graceful-way-to-handle-toggling-between-fullscreen-and-windowed-mode

    restoreDisplaySettings();

    // NOTE: do not use AdjustWindowRect.. 
    // It somehow messes up the position restore (client origin instead of 
    // window origin). Working solution here: 

    MoveWindow(m_hWnd, m_backup.left, m_backup.top, m_backup.width, m_backup.height, TRUE);

    SetWindowLongPtr(m_hWnd, GWL_STYLE
        , WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    m_mode = WindowMode;
}

void Window::toggleMode()
{
    switch (m_mode)
    {
    case TransitionMode:
        return;
    case FullScreenMode:
        return windowed();
    case WindowMode:
        return fullScreen();
    }
}

void Window::restoreDisplaySettings()
{
    const LONG result = ChangeDisplaySettings(nullptr, NULL);   // Return to the default mode after a dynamic mode change.
    if (DISP_CHANGE_SUCCESSFUL == result)
        return;

    fatal() << "Toggling to windowed mode failed (ChangeDisplaySettings).";
    printChangeDisplaySettingsErrorResult(result);
}

LRESULT CALLBACK Window::dispatch(
    HWND hWnd
,   UINT message
,   WPARAM wParam
,   LPARAM lParam)
{
    assert(!m_hWnd || hWnd == m_hWnd);

    // Windows Messages: http://msdn.microsoft.com/en-us/library/windows/desktop/ms644927(v=vs.85).aspx#windows_messages
    switch (message)
    {
    case WM_CLOSE:
        onClose();
        break;

    case WM_DESTROY:
        onDestroy();
        break;

    case WM_USER_VALID_CONTEXT:
        onValidContext();
        break;

    case WM_SIZE:
        onResize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_PAINT:
        onRepaint();
        break;

    case WM_USER_IDLE:
        onIdle();
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if(!onKeyPress(LOWORD(wParam)))
            DefWindowProc(hWnd, message, wParam, lParam);
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        if(!onKeyRelease(LOWORD(wParam)))
            DefWindowProc(hWnd, message, wParam, lParam);
        break;

    //case WM_ACTIVATE:
    //    // Check for minimization.
    //    if (!HIWORD(wParam))
    //        m_eventHandler->activateEvent();
    //    else
    //        m_eventHandler->minimizeEvent();
    //    break;

    case WM_SYSCOMMAND: 
        // Absorb some system commands.
        switch (wParam)
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
            return 0;
        }

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

bool Window::onKeyPress(const unsigned short key)
{
    KeyEvent kpe(KeyEvent::KeyPress, key);
    

    m_keysPressed.insert(kpe.key());
    return kpe.isAccepted();
}


bool Window::onKeyRelease(const unsigned short key)
{
    KeyEvent kre(KeyEvent::KeyRelease, key);

    switch (kre.key())
    {
    case KeyEvent::KeyEscape:
        kre.accept();
        SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
        break;

    case KeyEvent::KeyReturn:
        if (m_keysPressed.find(KeyEvent::KeyAlt) != m_keysPressed.cend())
        {
            kre.accept();
            toggleMode();
        }
        break;

    default: 
        break;
    }

    const auto f = m_keysPressed.find(kre.key());
    if (f != m_keysPressed.cend())
        m_keysPressed.erase(f);

    return kre.isAccepted();
}

} // namespace glow
