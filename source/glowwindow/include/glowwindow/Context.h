#pragma once

#include <string>

#include <glowwindow/glowwindow.h>
#include <glowwindow/ContextFormat.h>

struct GLFWwindow;
struct GLFWmonitor;

namespace glowwindow {

class AbstractNativeContext;

class GLOWWINDOW_API Context {

public:

    enum SwapInterval {
        NoVerticalSyncronization =  0,
        VerticalSyncronization = 1,
        AdaptiveVerticalSyncronization = -1
    };

    static const std::string swapIntervalString(SwapInterval swapInterval);

public:

    Context();
    virtual ~Context();

    GLFWwindow * window();

    /** Tries to create a context with the given format on the given handle.
     If successfull, m_format is set to the format created.

     \return isValid() is returned
     */
    bool create(const ContextFormat & format, int width, int height, GLFWmonitor * monitor = nullptr);
    void release();

    void makeCurrent();
    void doneCurrent();

    void swap();

    /** The returned format refers to the created context, not the requested one.
     */
    const ContextFormat & format() const;

    /** \return true if the context was created (irrespective of format
     verification) and if id() returns id > 0.
     */
    bool isValid() const;

    /** Swap interval relates to the context, since there might be multiple
     shared contexts with same format, but individual swap format.
     */
    void setSwapInterval(SwapInterval interval);
    SwapInterval swapInterval() const;

protected:
    SwapInterval m_swapInterval;
    ContextFormat m_format;

private:
    GLFWwindow * m_window;

private:
    static glow::Version maximumSupportedVersion();
    static glow::Version validateVersion(const glow::Version & version);
    void prepareFormat(const ContextFormat & format);

    static void handleError(int errorCode, const char* errorMessage);
};

} // namespace glowwindow
