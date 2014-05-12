#include <glbinding/constants.h>



#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/debugmessageoutput.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
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

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glow::debugmessageoutput::enable();

        gl::ClearColor(0.2f, 0.3f, 0.4f, 1.f);

    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        gl::Viewport(0, 0, event.width(), event.height());

    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }
};

/**
 * This example shows how to create a single window, probably in fullscreen
 * mode. All window and system events are handled with the event handler.
 */
int main(int /*argc*/, char* /*argv*/[])
{
    glow::info() << "Usage:";
    glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Single Context Example"))
    {
        window.context()->setSwapInterval(Context::VerticalSyncronization);

        window.show();

        return MainLoop::run();
    }
    else
    {
        return 1;
    }
}
