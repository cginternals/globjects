#pragma once

#include <set>
#include <string>

#include <glowwindow/glowwindow.h>

namespace glowwindow
{

class GLOWWINDOW_API MainLoop
{
protected:
    MainLoop();

public:
    /** This enters the (main) windows message loop.
    */
    static int run();
    static void quit(int code = 0);

public:
    void start();
    void stop(int code = 0);
    int exitCode();
    bool isRunning() const;

protected:
    bool m_running;
    int m_exitCode;

    void pollEvents();
    void processEvents();

protected:
    static MainLoop s_mainLoop;
};

} // namespace glowwindow
