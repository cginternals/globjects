
#include <iostream>

#include <glow/Screen.h>
#include <glow/Window.h>

using namespace glow;

int main(int argc, char** argv)
{
    unsigned int width, height;
    Screen::getDesktopResolution(width, height);

    glow::Window w;
    w.create("test", width, height);
    w.fullScreen();
    
    Sleep(2000);

    return 0;
}
