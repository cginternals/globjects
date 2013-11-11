#include <cassert>
#include <cmath>

#include <GLFW/glfw3.h>

#include <glow/logging.h>

#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include "WindowEventDispatcher.h"

namespace glow
{

void WindowEventDispatcher::registerWindow(Window* window)
{
    assert(window != nullptr);

    GLFWwindow* glfwWindow = window->internalWindow();

    if (!glfwWindow)
        return;

    glfwSetWindowUserPointer(glfwWindow, window);

    glfwSetWindowRefreshCallback(glfwWindow, processRefresh);
    glfwSetKeyCallback(glfwWindow, processKey);
    glfwSetCharCallback(glfwWindow, processChar);
    glfwSetMouseButtonCallback(glfwWindow, processMouse);
    glfwSetCursorPosCallback(glfwWindow, processCursorPos);
    glfwSetCursorEnterCallback(glfwWindow, processCursorEnter);
    glfwSetScrollCallback(glfwWindow, processScroll);
    glfwSetWindowSizeCallback(glfwWindow, processResize);
    glfwSetFramebufferSizeCallback(glfwWindow, processFramebufferResize);
    glfwSetWindowFocusCallback(glfwWindow, processFocus);
    glfwSetWindowPosCallback(glfwWindow, processMove);
    glfwSetWindowIconifyCallback(glfwWindow, processIconify);
    glfwSetWindowCloseCallback(glfwWindow, processClose);
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
}

Window* WindowEventDispatcher::fromGLFW(GLFWwindow* glfwWindow)
{
    return glfwWindow ? static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow)) : nullptr;
}

void WindowEventDispatcher::mousePosition(GLFWwindow* glfwWindow, int& x, int &y)
{
    if (!glfwWindow)
    {
        x = y = 0;
    }
    else
    {
        double xd, yd;
        glfwGetCursorPos(glfwWindow, &xd, &yd);
        x = std::floor(xd);
        y = std::floor(yd);
    }
}

void WindowEventDispatcher::dispatchEvent(GLFWwindow* glfwWindow, WindowEvent& event)
{
    Window* window = fromGLFW(glfwWindow);
    if (!window)
        return;

    window->postEvent(event);
}

// events

void WindowEventDispatcher::processRefresh(GLFWwindow* glfwWindow)
{
    PaintEvent event;
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int modifiers)
{
    KeyEvent event(key, scanCode, action, modifiers);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processChar(GLFWwindow* glfwWindow, unsigned int character)
{
    KeyEvent event(character);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processMouse(GLFWwindow* glfwWindow, int button, int action, int modifiers)
{
    int x, y;
    mousePosition(glfwWindow, x, y);

    MouseEvent event(x, y, button, action, modifiers);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processCursorPos(GLFWwindow* glfwWindow, double xPos, double yPos)
{
    MouseEvent event(std::floor(xPos), std::floor(yPos));
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processCursorEnter(GLFWwindow* glfwWindow, int entered)
{
    // TODO: implement
    //sendEvent(glfwWindow, event);
}

void WindowEventDispatcher::processScroll(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
    int x, y;
    mousePosition(glfwWindow, x, y);

    ScrollEvent event(xOffset, yOffset, x, y);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processResize(GLFWwindow* glfwWindow, int width, int height)
{
    ResizeEvent event(width, height);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processFramebufferResize(GLFWwindow* glfwWindow, int width, int height)
{
    ResizeEvent event(width, height, true);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processMove(GLFWwindow* glfwWindow, int x, int y)
{
    MoveEvent event(x, y);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processFocus(GLFWwindow* glfwWindow, int focused)
{
    FocusEvent event(focused == GL_TRUE);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processIconify(GLFWwindow* glfwWindow, int iconified)
{
    IconifyEvent event(iconified == GL_TRUE);
    dispatchEvent(glfwWindow, event);
}

void WindowEventDispatcher::processClose(GLFWwindow* glfwWindow)
{
    CloseEvent event;
    dispatchEvent(glfwWindow, event);
}

} // namespace glow
