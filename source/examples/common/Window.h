#pragma once

#include <set>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <common/MainLoop.h>


struct GLFWwindow;
struct GLFWmonitor;

class WindowEventHandler;
class WindowEvent;
class ContextFormat;
class Context;


/** Attach a WindowEventHandler specialization for event handling.
*/
class Window
{
public:
    static int init();

    static const std::set<Window *> & instances();

public:
    Window();
    virtual ~Window();

    bool create(
        const ContextFormat & format
        , int width = 1280
        , int height = 720);
    bool create(
        const ContextFormat & format
        , const std::string & title = "globjects"
        , int width = 1280
        , int height = 720);

    void setTitle(const std::string & title);
    const std::string & title() const;

    Context * context() const;
    GLFWwindow * internalWindow() const;

    void destroy();

    /** If enabled, this causes an application wide quit message to be posted
    when the window gets destroyed. Hence, the MainLoop will be quit
    and all other remaining windows destroyed.
    */
    void setQuitOnDestroy(bool enable);
    bool quitsOnDestroy() const;

    /** Takes ownership of the given eventhandler and deletes that either on
        quitting, just before the opengl context gets destroyed, or when
        reassigning a new, different handler.
    */
    void setEventHandler(WindowEventHandler * eventHandler);

    const WindowEventHandler * eventHandler() const;
    WindowEventHandler * eventHandler();

    void show();
    void hide();
    void close();

    void resize(int width, int height);
    glm::ivec2 size() const;
    glm::ivec2 framebufferSize() const;

    glm::ivec2 position() const;

    void setInputMode(int mode, int value);
    int inputMode(int mode) const;


    void repaint();
    void idle();
    void swap();

    void fullScreen();
    bool isFullScreen() const;

    void windowed();
    bool isWindowed() const;

    void toggleMode();
    void toggleVSync();

    void queueEvent(WindowEvent * event);

    bool hasPendingEvents();
    void processEvents();

    void addTimer(int id, int interval, bool singleShot = false);
    void removeTimer(int id);

protected:
    bool createContext(
        const ContextFormat & format
    ,   bool verify
    ,   int width
    ,   int height
    ,   GLFWmonitor * monitor = nullptr);

    void destroyContext();

    void initializeEventHandler();
    void finalizeEventHandler();

    void clearEventQueue();
    void processEvent(WindowEvent & event);
    void postprocessEvent(WindowEvent & event);

    enum class Mode
    {
        Windowed
    ,   FullScreen
    };

    void setMode(Mode mode);

protected:
    Context * m_context;
    GLFWwindow * m_window;

    WindowEventHandler * m_eventHandler;

    using EventQueue = std::vector<WindowEvent *>;
    EventQueue m_eventQueue[2];

    EventQueue * m_activeEventQueue;
    EventQueue * m_inactiveEventQueue;

    glm::ivec2 m_windowedModeSize;
    glm::ivec2 m_windowedModePosition;
    std::string m_title;

    bool m_quitOnDestroy;

    Mode m_mode;

private:
    static std::set<Window *> s_instances;
};
