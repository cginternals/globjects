#pragma once

#include <set>

struct GLFWwindow;

namespace glow
{

class Window;

class WindowEventDispatcher
{
public:
    static void registerWindow(Window* window);
    static void deregisterWindow(Window* window);
private:
    WindowEventDispatcher();

protected:
    static Window* fromGLFW(GLFWwindow* glfwWindow);
    static void mousePosition(GLFWwindow* glfwWindow, int& x, int &y);

    static void queueEvent(GLFWwindow* glfwWindow, WindowEvent* event);

    static void handleRefresh(GLFWwindow* glfwWindow);
    static void handleKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int modifiers);
    static void handleChar(GLFWwindow* glfwWindow, unsigned int character);
    static void handleMouse(GLFWwindow* glfwWindow, int button, int action, int modifiers);
    static void handleCursorPos(GLFWwindow* glfwWindow, double xPos, double yPos);
    static void handleCursorEnter(GLFWwindow* glfwWindow, int entered);
    static void handleScroll(GLFWwindow* glfwWindow, double xOffset, double yOffset);
    static void handleResize(GLFWwindow* glfwWindow, int width, int height);
    static void handleFramebufferResize(GLFWwindow* glfwWindow, int width, int height);
    static void handleMove(GLFWwindow* glfwWindow, int x, int y);
    static void handleFocus(GLFWwindow* glfwWindow, int focused);
    static void handleIconify(GLFWwindow* glfwWindow, int iconified);
    static void handleClose(GLFWwindow* glfwWindow);
};

} // namespace glow
