
#include <GL/glew.h>

#include <glow/AutoTimer.h>
#include <glowwindow/ContextFormat.h>
#include <glow/Error.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include <glow/logging.h>

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

    virtual void initialize(Window & window) override
    {
        DebugMessageOutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);
    }
    
    virtual void resizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }
};

/** This example shows how to create a single window, probably in fullscreen
    mode. All window and system events are handled with the event handler.
*/
int main(int argc, char* argv[])
{
    ContextFormat format;
    format.setVersion(4, 3);
    format.setProfile(ContextFormat::CoreProfile);

    Window window;
    {
    AutoTimer t("Initialization");

    window.setEventHandler(new EventHandler());
    if (!window.create(format, "Single Context Example"))
        return 0;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();
    }
    return MainLoop::run();
}
