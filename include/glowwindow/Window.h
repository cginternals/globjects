#pragma once

#include <set>
#include <string>
#include <queue>

#include <glow/ref_ptr.h>

#include <glowwindow/glowwindow.h>
#include <glowwindow/MainLoop.h>
#include <glowwindow/events.h>  // forward?


struct GLFWwindow;

namespace glow
{
    class Timer;
}

namespace glowwindow 
{
class WindowEventHandler;
class ContextFormat;
class Context;

/**

    Attach a WindowEventHandler specialization for event handling.
*/
class GLOWWINDOW_API Window
{
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
    void quitOnDestroy(bool enable);
    bool quitsOnDestroy() const;

    /** Takes ownership of the given eventhandler and deletes that either on
        quitting, just before the opengl context gets destroyed, or when
        reassigning a new, different handler. 
    */
    void setEventHandler(WindowEventHandler * eventHandler);

    bool create(
        const ContextFormat & format
    ,   const std::string & title = "glow"
    ,   int width  =  1280
    ,   int height =   720);

    void setTitle(const std::string & title);

    void close();

    /** Queues a paint event.
    */
    void repaint();

    void resize(int width, int height);

    void idle();

    void show();
    void hide();

    void fullScreen();
    bool isFullScreen() const;
    void windowed();
    bool isWindowed() const;

    void toggleMode();

    GLFWwindow * internalWindow() const;

    void queueEvent(WindowEvent * event);
    bool hasPendingEvents();
    void processEvents();

    static const std::set<Window*>& instances();

    void addTimer(int id, int interval, bool singleShot = false);
    void removeTimer(int id);

protected:
    void swap();
    void destroy();

    void promoteContext();

    void clearEventQueue();
    void processEvent(WindowEvent & event);
    void finishEvent(WindowEvent & event);
    void defaultEventAction(WindowEvent & event);

protected:
    glow::ref_ptr<WindowEventHandler> m_eventHandler;
    Context * m_context;
    std::queue<WindowEvent*> m_eventQueue;

    bool m_quitOnDestroy;

    enum Mode
    {
        WindowMode
    ,   FullScreenMode
    ,   TransitionMode
    };

    Mode m_mode;

    std::string m_title;

    glow::Timer * m_timer;

    long double m_swapts;
    unsigned int m_swaps;

private:
    GLFWwindow * m_window;
    static std::set<Window*> s_instances;

    int m_width;
    int m_height;
};

} // namespace glowwindow
