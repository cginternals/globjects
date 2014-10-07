#pragma once

class MainLoop
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

    bool isRunning() const;

    int exitCode();

protected:
    int m_exitCode;
    bool m_running;

    void pollEvents();
    void processEvents();

protected:
    static MainLoop s_mainLoop;
};
