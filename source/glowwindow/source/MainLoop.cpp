#include <glowwindow/MainLoop.h>

#include <GLFW/glfw3.h>

#include <glowwindow/Window.h>
#include "WindowEventDispatcher.h"

namespace glowwindow
{

MainLoop MainLoop::s_mainLoop;

MainLoop::MainLoop()
: m_exitCode(0)
, m_running(false)
{
}

int MainLoop::run()
{
    if (s_mainLoop.isRunning())
        return 1;

    s_mainLoop.start();

    return s_mainLoop.exitCode();
}

void MainLoop::quit(int code)
{
    s_mainLoop.stop(code);
}

void MainLoop::start()
{
    m_running = true;

    WindowEventDispatcher::initializeTime();

    while (m_running)
    {
        pollEvents();
        processEvents();
    };

    glfwTerminate();
}

void MainLoop::stop(int code)
{
    m_exitCode = code;
    m_running = false;
}

bool MainLoop::isRunning() const
{
    return m_running;
}

int MainLoop::exitCode()
{
    return m_exitCode;
}

void MainLoop::pollEvents()
{
    glfwPollEvents();
    WindowEventDispatcher::checkForTimerEvents();
}

void MainLoop::processEvents()
{
    for (Window* window : Window::instances())
    {
        if (window->hasPendingEvents())
            window->processEvents();
        else
            window->idle();
    }
}

} // namespace glowwindow
