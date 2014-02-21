
#include <iostream>

#include <GL/glew.h>

#include <glow/Error.h>
#include <glow/debugmessageoutput.h>

#include <glowwindow/Window.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;

class EventHandler : public ExampleWindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & ) override
    {
        glow::debugmessageoutput::enable();

        glClearColor(1.f, 1.f, 1.f, 1.f);
        CheckGLError();
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        CheckGLError();
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClearColor(
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            1.f);
        CheckGLError();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CheckGLError();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }
};

/** This example shows how to create multiple windows with each having its own
    OpenGL context. Further, the events of all windows are handled within a 
    single event handler. This allows, e.g., reuse of painting functionality
    with minor, windows specific variations (e.g., different views).
*/
int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 0);

    Window windows[8];

    for (int i = 0; i < 8; ++i)
    {
        windows[i].setEventHandler(new EventHandler());

        if (!windows[i].create(format, "Multiple Contexts Example", 320, 240))
        {
            return 1;
        }

        windows[i].show();
        windows[i].context()->setSwapInterval(Context::NoVerticalSyncronization);

        // make some random windows post quit on destroy ;)
        windows[i].quitOnDestroy(0 != rand() % 2);
    }

    return MainLoop::run();
}
