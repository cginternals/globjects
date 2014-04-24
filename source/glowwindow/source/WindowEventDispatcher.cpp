#include "WindowEventDispatcher.h"

#include <cassert>
#include <cmath>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

namespace glowwindow
{

WindowEventDispatcher::Timer::Timer()
: interval(0)
, singleShot(false)
{
    reset();
}

WindowEventDispatcher::Timer::Timer(int interval, bool singleShot)
: interval(interval)
, singleShot(singleShot)
{
    reset();
}

bool WindowEventDispatcher::Timer::ready() const
{
    return elapsed >= interval;
}

void WindowEventDispatcher::Timer::reset()
{
    elapsed = Duration(0);
}

WindowEventDispatcher::WindowTimerMap WindowEventDispatcher::s_timers;
std::chrono::high_resolution_clock::time_point WindowEventDispatcher::s_time;
std::chrono::high_resolution_clock WindowEventDispatcher::s_clock;

void WindowEventDispatcher::registerWindow(Window* window)
{
    assert(window != nullptr);

    GLFWwindow* glfwWindow = window->internalWindow();

    if (!glfwWindow)
        return;

    glfwSetWindowUserPointer(glfwWindow, window);

    glfwSetWindowRefreshCallback(glfwWindow, handleRefresh);
    glfwSetKeyCallback(glfwWindow, handleKey);
    glfwSetCharCallback(glfwWindow, handleChar);
    glfwSetMouseButtonCallback(glfwWindow, handleMouse);
    glfwSetCursorPosCallback(glfwWindow, handleCursorPos);
    glfwSetCursorEnterCallback(glfwWindow, handleCursorEnter);
    glfwSetScrollCallback(glfwWindow, handleScroll);
    glfwSetWindowSizeCallback(glfwWindow, handleResize);
    glfwSetFramebufferSizeCallback(glfwWindow, handleFramebufferResize);
    glfwSetWindowFocusCallback(glfwWindow, handleFocus);
    glfwSetWindowPosCallback(glfwWindow, handleMove);
    glfwSetWindowIconifyCallback(glfwWindow, handleIconify);
    glfwSetWindowCloseCallback(glfwWindow, handleClose);
}

void WindowEventDispatcher::deregisterWindow(Window* window)
{
    assert(window != nullptr);

    GLFWwindow* glfwWindow = window->internalWindow();

    if (!glfwWindow)
        return;

    glfwSetWindowRefreshCallback(glfwWindow, nullptr);
    glfwSetKeyCallback(glfwWindow, nullptr);
    glfwSetCharCallback(glfwWindow, nullptr);
    glfwSetMouseButtonCallback(glfwWindow, nullptr);
    glfwSetCursorPosCallback(glfwWindow, nullptr);
    glfwSetCursorEnterCallback(glfwWindow, nullptr);
    glfwSetScrollCallback(glfwWindow, nullptr);
    glfwSetWindowSizeCallback(glfwWindow, nullptr);
    glfwSetFramebufferSizeCallback(glfwWindow, nullptr);
    glfwSetWindowFocusCallback(glfwWindow, nullptr);
    glfwSetWindowPosCallback(glfwWindow, nullptr);
    glfwSetWindowIconifyCallback(glfwWindow, nullptr);
    glfwSetWindowCloseCallback(glfwWindow, nullptr);

    removeTimers(window);
}

void WindowEventDispatcher::addTimer(Window* window, int id, int interval, bool singleShot)
{
    s_timers[window][id] = Timer(interval, singleShot);
}

void WindowEventDispatcher::removeTimer(Window* window, int id)
{
    s_timers[window].erase(id);
}

void WindowEventDispatcher::removeTimers(Window* window)
{
    s_timers.erase(window);
}

void WindowEventDispatcher::initializeTime()
{
    s_time = s_clock.now();
}

void WindowEventDispatcher::checkForTimerEvents()
{
    auto now = s_clock.now();
    Timer::Duration delta = std::chrono::duration_cast<Timer::Duration>(now - s_time);
    s_time = now;

    std::vector<std::pair<Window*, int>> discarded;

    for (auto& timerMapPair : s_timers)
    {
        Window* window = timerMapPair.first;
        for (auto& timerPair : timerMapPair.second)
        {
            int id = timerPair.first;
            Timer& timer = timerPair.second;

            timer.elapsed += delta;

            if (timer.ready())
            {
                dispatchEvent(window, new TimerEvent(id));
                if (timer.singleShot)
                {
                    discarded.emplace_back(window, id);
                }
                else
                {
                    timer.reset();
                }
            }
        }
    }

    for (const auto& pair : discarded)
    {
        removeTimer(pair.first, pair.second);
    }
}

Window* WindowEventDispatcher::fromGLFW(GLFWwindow* glfwWindow)
{
    return glfwWindow ? static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow)) : nullptr;
}

glm::ivec2 WindowEventDispatcher::mousePosition(GLFWwindow* glfwWindow)
{
    if (glfwWindow)
    {
        double xd, yd;
        glfwGetCursorPos(glfwWindow, &xd, &yd);

        return glm::ivec2(std::floor(xd), std::floor(yd));
    }
    else
    {
        return glm::ivec2();
    }
}

void WindowEventDispatcher::dispatchEvent(GLFWwindow* glfwWindow, WindowEvent* event)
{
    dispatchEvent(fromGLFW(glfwWindow), event);
}

void WindowEventDispatcher::dispatchEvent(Window* window, WindowEvent* event)
{
    if (!window)
    {
        delete event;
        return;
    }

    window->queueEvent(event);
}

// events

void WindowEventDispatcher::handleRefresh(GLFWwindow* glfwWindow)
{
    dispatchEvent(glfwWindow, new PaintEvent);
}

void WindowEventDispatcher::handleKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int modifiers)
{
    dispatchEvent(glfwWindow, new KeyEvent(key, scanCode, action, modifiers));
}

void WindowEventDispatcher::handleChar(GLFWwindow* glfwWindow, unsigned int character)
{
    dispatchEvent(glfwWindow, new KeyEvent(character));
}

void WindowEventDispatcher::handleMouse(GLFWwindow* glfwWindow, int button, int action, int modifiers)
{
    dispatchEvent(glfwWindow, new MouseEvent(mousePosition(glfwWindow), button, action, modifiers));
}

void WindowEventDispatcher::handleCursorPos(GLFWwindow* glfwWindow, double xPos, double yPos)
{
    dispatchEvent(glfwWindow, new MouseEvent(glm::ivec2(std::floor(xPos), std::floor(yPos))));
}

void WindowEventDispatcher::handleCursorEnter(GLFWwindow* /*glfwWindow*/, int /*entered*/)
{
    // TODO: implement
}

void WindowEventDispatcher::handleScroll(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
    dispatchEvent(glfwWindow, new ScrollEvent(glm::vec2(xOffset, yOffset), mousePosition(glfwWindow)));
}

void WindowEventDispatcher::handleResize(GLFWwindow* glfwWindow, int width, int height)
{
    dispatchEvent(glfwWindow, new ResizeEvent(glm::ivec2(width, height)));
}

void WindowEventDispatcher::handleFramebufferResize(GLFWwindow* glfwWindow, int width, int height)
{
    dispatchEvent(glfwWindow, new ResizeEvent(glm::ivec2(width, height), true));
}

void WindowEventDispatcher::handleMove(GLFWwindow* glfwWindow, int x, int y)
{
    dispatchEvent(glfwWindow, new MoveEvent(glm::ivec2(x, y)));
}

void WindowEventDispatcher::handleFocus(GLFWwindow* glfwWindow, int focused)
{
    dispatchEvent(glfwWindow, new FocusEvent(focused == GL_TRUE));
}

void WindowEventDispatcher::handleIconify(GLFWwindow* glfwWindow, int iconified)
{
    dispatchEvent(glfwWindow, new IconifyEvent(iconified == GL_TRUE));
}

void WindowEventDispatcher::handleClose(GLFWwindow* glfwWindow)
{
    dispatchEvent(glfwWindow, new CloseEvent);
}

} // namespace glowwindow
