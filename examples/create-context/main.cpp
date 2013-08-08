
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

    virtual void initializeEvent()
    {
        glClearColor(1.f, 1.f, 1.f, 1.f);
    }
    
    virtual void resizeEvent(
        const unsigned int width
    ,   const unsigned int height)
    {
        glViewport(0, 0, width, height);
    }

    virtual void paintEvent()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};

int main(int argc, char** argv)
{
    unsigned int width, height;
    Screen::getDesktopResolution(width, height);

    ContextFormat format;
    EventHandler events;

    Window window;

    window.create(format, "Create Context Example", 1280, 720);
    window.show();

    return window.run(&events);
}
