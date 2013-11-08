
#include <GLFW/glfw3.h>

#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include "WindowEventDispatcher.h"

namespace glow
{

void WindowEventDispatcher::registerWindow(Window* window)
{
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
    return static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
}

void WindowEventDispatcher::sendEvent(GLFWwindow* glfwWindow, WindowEvent* event)
{
    Window* window = fromGLFW(glfwWindow);
    if (!window)
        return;

    window->processEvent(event);
}

// events

void WindowEventDispatcher::processRefresh(GLFWwindow* glfwWindow)
{
    WindowEvent event(WindowEvent::Paint);
    sendEvent(glfwWindow, &event);
}

void WindowEventDispatcher::processKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int modifiers)
{
    KeyEvent event(key, scanCode, action, modifiers);
    sendEvent(glfwWindow, &event);
}

void WindowEventDispatcher::processChar(GLFWwindow* glfwWindow, unsigned int character)
{

}

void WindowEventDispatcher::processMouse(GLFWwindow* glfwWindow, int button, int action, int modifiers)
{

}

void WindowEventDispatcher::processCursorPos(GLFWwindow* glfwWindow, double xPos, double yPos)
{

}

void WindowEventDispatcher::processCursorEnter(GLFWwindow* glfwWindow, int entered)
{

}

void WindowEventDispatcher::processScroll(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{

}

void WindowEventDispatcher::processResize(GLFWwindow* glfwWindow, int width, int height)
{
    ResizeEvent event(width, height);
    sendEvent(glfwWindow, &event);
}

void WindowEventDispatcher::processFramebufferResize(GLFWwindow* glfwWindow, int width, int height)
{

}

void WindowEventDispatcher::processMove(GLFWwindow* glfwWindow, int x, int y)
{

}

void WindowEventDispatcher::processFocus(GLFWwindow* glfwWindow, int focused)
{

}

void WindowEventDispatcher::processIconify(GLFWwindow* glfwWindow, int iconified)
{

}

void WindowEventDispatcher::processClose(GLFWwindow* glfwWindow)
{
    WindowEvent event(WindowEvent::Close);
    sendEvent(glfwWindow, &event);
}

} // namespace glow
