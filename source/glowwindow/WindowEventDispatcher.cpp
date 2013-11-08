#include <cassert>

#include <GLFW/glfw3.h>

#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include "WindowEventDispatcher.h"

namespace glow
{

void WindowEventDispatcher::registerWindow(Window* window)
{
    assert(window != nullptr);

    GLFWwindow* glfwWindow = window->m_window;

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

    GLFWwindow* glfwWindow = window->m_window;

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
    assert(glfwWindow != nullptr);

    return static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
}

void WindowEventDispatcher::sendEvent(GLFWwindow* glfwWindow, WindowEvent* event)
{
    assert(glfwWindow != nullptr);

    Window* window = fromGLFW(glfwWindow);
    if (!window)
        return;

    window->processEvent(event);
}

// events

void WindowEventDispatcher::processRefresh(GLFWwindow* glfwWindow)
{
    assert(glfwWindow != nullptr);

    WindowEvent event(WindowEvent::Paint);
    sendEvent(glfwWindow, &event);
}

void WindowEventDispatcher::processKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int modifiers)
{
    assert(glfwWindow != nullptr);

    KeyEvent event(key, scanCode, action, modifiers);
    sendEvent(glfwWindow, &event);
}

void WindowEventDispatcher::processChar(GLFWwindow* glfwWindow, unsigned int character)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processMouse(GLFWwindow* glfwWindow, int button, int action, int modifiers)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processCursorPos(GLFWwindow* glfwWindow, double xPos, double yPos)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processCursorEnter(GLFWwindow* glfwWindow, int entered)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processScroll(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processResize(GLFWwindow* glfwWindow, int width, int height)
{
    assert(glfwWindow != nullptr);

    ResizeEvent event(width, height);
    sendEvent(glfwWindow, &event);
}

void WindowEventDispatcher::processFramebufferResize(GLFWwindow* glfwWindow, int width, int height)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processMove(GLFWwindow* glfwWindow, int x, int y)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processFocus(GLFWwindow* glfwWindow, int focused)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processIconify(GLFWwindow* glfwWindow, int iconified)
{
    assert(glfwWindow != nullptr);
}

void WindowEventDispatcher::processClose(GLFWwindow* glfwWindow)
{
    assert(glfwWindow != nullptr);

    WindowEvent event(WindowEvent::Close);
    sendEvent(glfwWindow, &event);
}

} // namespace glow
