#pragma once

#include <unordered_map>
#include <chrono>

#include <glm/glm.hpp>

struct GLFWwindow;

namespace glowwindow
{
    
class Window;
class WindowEvent;

class WindowEventDispatcher
{
public:
    struct Timer
    {
        using Duration = std::chrono::duration<double, std::milli>;

        Timer();
        Timer(int interval, bool singleShot);

        std::chrono::milliseconds interval;
        Duration elapsed;
        bool singleShot;

        bool ready() const;
        void reset();
    };
public:
    static void registerWindow(Window* window);
    static void deregisterWindow(Window* window);

    static void addTimer(Window* window, int id, int interval, bool singleShot);
    static void removeTimer(Window* window, int id);
    static void removeTimers(Window* window);
    static void initializeTime();
    static void checkForTimerEvents();
private:
    WindowEventDispatcher();

    using TimerMap = std::unordered_map<int, Timer>;
    using WindowTimerMap = std::unordered_map<Window*, TimerMap>;

    static WindowTimerMap s_timers;
    static std::chrono::high_resolution_clock::time_point s_time;
    static std::chrono::high_resolution_clock s_clock;
protected:
    static Window* fromGLFW(GLFWwindow* glfwWindow);
    static glm::ivec2 mousePosition(GLFWwindow* glfwWindow);

    static void dispatchEvent(GLFWwindow* glfwWindow, WindowEvent* event);
    static void dispatchEvent(Window* window, WindowEvent* event);

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
