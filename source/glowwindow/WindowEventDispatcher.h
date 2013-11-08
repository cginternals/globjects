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
    static void sendEvent(GLFWwindow* glfwWindow, WindowEvent* event);

    static void processRefresh(GLFWwindow* glfwWindow);
    static void processKey(GLFWwindow* glfwWindow, int key, int scanCode, int action, int modifiers);
    static void processChar(GLFWwindow* glfwWindow, unsigned int character);
    static void processMouse(GLFWwindow* glfwWindow, int button, int action, int modifiers);
    static void processCursorPos(GLFWwindow* glfwWindow, double xPos, double yPos);
    static void processCursorEnter(GLFWwindow* glfwWindow, int entered);
    static void processScroll(GLFWwindow* glfwWindow, double xOffset, double yOffset);
    static void processResize(GLFWwindow* glfwWindow, int width, int height);
    static void processFramebufferResize(GLFWwindow* glfwWindow, int width, int height);
    static void processMove(GLFWwindow* glfwWindow, int x, int y);
    static void processFocus(GLFWwindow* glfwWindow, int focused);
    static void processIconify(GLFWwindow* glfwWindow, int iconified);
    static void processClose(GLFWwindow* glfwWindow);
};

} // namespace glow
