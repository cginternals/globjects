#include <cassert>
#include <cmath>

#include <thread>

#include <GLFW/glfw3.h>

#include <glow/logging.h>

#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include "WindowEventDispatcher.h"

namespace glow
{

WindowEventDispatcher::Timer::Timer()
: interval(0)
, singleShot(false)
{
}

WindowEventDispatcher::Timer::Timer(int interval, bool singleShot)
: interval(interval)
, singleShot(singleShot)
, delapsed(0)
{
}

bool WindowEventDispatcher::Timer::ready() const
{
//    int a = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
//    int b = elapsed.count();
////    glow::debug() << b << " >= " << interval << " -> " << (b >= interval);
//    return b >= interval;
    return delapsed >= interval;
}

void WindowEventDispatcher::Timer::reset()
{
    elapsed %= interval;
    elapsed *= 0;
    delapsed = 0;
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

    dtime = glfwGetTime();
}

double WindowEventDispatcher::dtime = 0;

void WindowEventDispatcher::checkForTimerEvents()
{
//    auto foo = s_clock.now();
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    auto bar = s_clock.now() - foo;

//    double d = glfwGetTime();
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    d = glfwGetTime()-d;

//    int agzi;
   // std::chrono::nanoseconds uidghe = foo-foo;
//    auto arbd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(1));

//    std::chrono::milliseconds bar2 = std::chrono::duration_cast<std::chrono::milliseconds>(bar);

//    int a;

//    auto now = s_clock.now();
//    std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_time);
////    std::chrono::milliseconds delta = (now - s_time);
//    s_time = now;

//    delta*=1000;

    double now = glfwGetTime();
    double d = now-dtime;
    dtime = now;

    //auto delta = std::chrono::milliseconds(int(d*1000));

    std::vector<std::pair<Window*, int>> discarded;

    for (auto& timerMapPair : s_timers)
    {
        Window* window = timerMapPair.first;
        for (auto& timerPair : timerMapPair.second)
        {
            int id = timerPair.first;
            Timer& timer = timerPair.second;

//            timer.elapsed += delta;
            timer.delapsed += 1000*d;

            if (timer.ready())
            {
                dispatchEvent(window, new TimerEvent(id));
                if (timer.singleShot)
                {
                    discarded.push_back(std::pair<Window*, int>(window, id));
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

void WindowEventDispatcher::handleCursorEnter(GLFWwindow* glfwWindow, int entered)
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

} // namespace glow
