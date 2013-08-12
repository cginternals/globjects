
#include <iostream>

#include <GL/glew.h>

#include <glow/Screen.h>
#include <glow/Window.h>
#include <glow/ContextFormat.h>
#include <glow/WindowEventHandler.h>

using namespace glow;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initializeEvent(Window & window)
    {
        glClearColor(0.2f, 0.3f, 0.4f, 1.f);
    }
    
    virtual void resizeEvent(
        Window & window
    ,   const unsigned int width
    ,   const unsigned int height)
    {
        glViewport(2, 2, width - 4, height - 4);
    }

    virtual void paintEvent(Window & window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    virtual void idleEvent(Window & window)
    {
        window.repaint();
    }
};

/** This example shows how to create a single window, probably in fullscreen
    mode. All window and system events are handled with the event handler.
*/
int main(int argc, char** argv)
{
    ContextFormat format;
    EventHandler handler;

    Window window;
    window.attach(&handler);

    window.create(format, "Single Context Example", 1280, 800);
    window.show();
    window.fullScreen();
    window.windowed();

    return Window::run();
}
