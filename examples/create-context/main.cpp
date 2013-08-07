
#include <iostream>

#include <GL/glew.h>

#include <glow/Screen.h>
#include <glow/Window.h>
#include <glow/Context.h>

using namespace glow;

int main(int argc, char** argv)
{
    unsigned int width, height;
    Screen::getDesktopResolution(width, height);

    glow::Window w;
    w.create("test", width, height);
    w.show();

    Sleep(2000);

    glow::ContextFormat f;

    glow::Context c(w.handle());
    c.create(f);

    w.fullScreen();

    glViewport(0, 0, width, height);

    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    c.swap();

    Sleep(2000);

    return 0;
}
