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

void WindowEventDispatcher::queueEvent(GLFWwindow* glfwWindow, WindowEvent* event)
{
    Window* window = fromGLFW(glfwWindow);
    if (!window)
        return;

    window->queueEvent(event);
}

// events

void WindowEventDispatcher::handleRefresh(GLFWwindow* glfwWindow)
{
    queueEvent(glfwWindow, new PaintEvent);
}

void WindowEventDispatcher::handleKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int modifiers)
{
    queueEvent(glfwWindow, new KeyEvent(key, scanCode, action, modifiers));
}

void WindowEventDispatcher::handleChar(GLFWwindow* glfwWindow, unsigned int character)
{
    queueEvent(glfwWindow, new KeyEvent(character));
}

void WindowEventDispatcher::handleMouse(GLFWwindow* glfwWindow, int button, int action, int modifiers)
{
    int x, y;
    mousePosition(glfwWindow, x, y);

    queueEvent(glfwWindow, new MouseEvent(x, y, button, action, modifiers));
}

void WindowEventDispatcher::handleCursorPos(GLFWwindow* glfwWindow, double xPos, double yPos)
{
    queueEvent(glfwWindow, new MouseEvent(std::floor(xPos), std::floor(yPos)));
}

void WindowEventDispatcher::handleCursorEnter(GLFWwindow* glfwWindow, int entered)
{
    // TODO: implement
    //queueEvent(glfwWindow, event);
}

void WindowEventDispatcher::handleScroll(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
    int x, y;
    mousePosition(glfwWindow, x, y);

    queueEvent(glfwWindow, new ScrollEvent(xOffset, yOffset, x, y));
}

void WindowEventDispatcher::handleResize(GLFWwindow* glfwWindow, int width, int height)
{
    queueEvent(glfwWindow, new ResizeEvent(width, height));
}

void WindowEventDispatcher::handleFramebufferResize(GLFWwindow* glfwWindow, int width, int height)
{
    queueEvent(glfwWindow, new ResizeEvent(width, height, true));
}

void WindowEventDispatcher::handleMove(GLFWwindow* glfwWindow, int x, int y)
{
    queueEvent(glfwWindow, new MoveEvent(x, y));
}

void WindowEventDispatcher::handleFocus(GLFWwindow* glfwWindow, int focused)
{
    queueEvent(glfwWindow, new FocusEvent(focused == GL_TRUE));
}

void WindowEventDispatcher::handleIconify(GLFWwindow* glfwWindow, int iconified)
{
    queueEvent(glfwWindow, new IconifyEvent(iconified == GL_TRUE));
}

void WindowEventDispatcher::handleClose(GLFWwindow* glfwWindow)
{
    queueEvent(glfwWindow, new CloseEvent);
}

} // namespace glow
