#pragma once

#include <glowwindow/glowwindow_api.h>

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
    int m_exitCode;
    bool m_running;

    void pollEvents();
    void processEvents();

protected:
    static MainLoop s_mainLoop;
};

} // namespace glowwindow
