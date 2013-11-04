#pragma once

#include <set>
#include <string>

#include <glm/glm.hpp>

#include <glowwindow/glowwindow.h>
#include <glowwindow/KeyEvent.h>

struct GLFWwindow;

namespace glow 
{

class WindowEventHandler;
class ContextFormat;
class Context;
class Timer;

/**

    Attach a WindowEventHandler specialization for event handling.
*/
class GLOWWINDOW_API Window
{
friend class AbstractNativeWindow;

public:
    Window();
    virtual ~Window();

    WindowEventHandler * eventHandler() const;
    Context * context() const;

    int width() const;
    int height() const;

    /** If enabled, this causes an application wide quit message to be posted
        when the window gets destroyed. Hence, the static window loop (run) 
        will receive a quit event and destroy all other remaining windows.
    */
    void setQuitOnDestroy(const bool enable);
    bool quitsOnDestroy() const;

    /** Takes ownership of the given eventhandler and deletes that either on
        quiting, just before the opengl context gets destroyed, or when 
        reassigning a new, different handler. 
    */
    void assign(WindowEventHandler * eventHandler);

    bool create(
        const ContextFormat & format
    ,   const std::string & title = "glow"
    ,   int width  =  1280
    ,   int height =   720);

    void close();

    /** Repaint triggers a frame while resolving/handling platform specific
        validation states and update regions.        
    */
    void repaint();

    void show();
    void hide();

    void fullScreen();
    bool isFullScreen() const;
    void windowed();
    bool isWindowed() const;

    void toggleMode();

public:
    /** This enters the (main) windows message loop and dispatches events to
        the attached WindowEventHandler instance.
    */
    static int run();
    static void quit(const int code = 0);

protected:
    void promoteContext();

    /** onRepaint makes the context valid, calls the paint event of the attached 
        event handler, swaps the buffer, and releases the context. 
    */
    void onRepaint();
    void onResize();

    void onIdle();
    void onClose();

    bool onKeyPress(const int key);
    bool onKeyRelease(const int key);

protected:
    void processKeyEvent(
        KeyEvent & event
    ,   WindowEventHandler * eventHandler);

protected:
    WindowEventHandler * m_eventHandler;
    Context * m_context;

    bool m_quitOnDestroy;

    std::set<int> m_keysPressed;

    enum Mode
    {
        WindowMode
    ,   FullScreenMode
    ,   TransitionMode
    };

    Mode m_mode;

    std::string m_title;

    Timer * m_timer;

    long double m_swapts;
    unsigned int m_swaps;

private:
    GLFWwindow * m_window;
    static std::set<Window*> s_windows;

    glm::ivec2 m_size;
};

} // namespace glow
