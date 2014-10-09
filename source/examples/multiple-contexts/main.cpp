
#include <iostream>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <common/Window.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace globjects;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClearColor(
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            1.f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};

/** This example shows how to create multiple windows with each having its own
    OpenGL context. Further, the events of all windows are handled within a 
    single event handler. This allows, e.g., reuse of painting functionality
    with minor, windows specific variations (e.g., different views).
*/
int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"       << "Toggle vertical sync";

    ContextFormat format;
    format.setVersion(3, 0);

    Window::init();

    Window windows[8];

    for (int i = 0; i < 8; ++i)
    {
        windows[i].setEventHandler(new EventHandler());

        if (!windows[i].create(format, "Multiple Contexts Example", 320, 240))
            return 1;

        windows[i].setQuitOnDestroy(i == 0 || rand() % 4 == 0);
        windows[i].show();
    }
    return MainLoop::run();
}
