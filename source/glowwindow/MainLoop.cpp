
#include <cassert>
#include <string>
#include <iomanip>

#include <GLFW/glfw3.h>

#include <glow/logging.h>
#include <glow/Timer.h>

#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/Window.h>
#include <glowwindow/events.h>

#include "WindowEventDispatcher.h"

#include <glowwindow/MainLoop.h>

namespace glow
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

    while (m_running)
    {
        idle();
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
}

void MainLoop::processEvents()
{
    for (Window* window : Window::instances())
    {
        window->processEvents();
    }
}

void MainLoop::idle()
{
    for (Window* window : Window::instances())
    {
        window->idle();
    }
}

} // namespace glow
