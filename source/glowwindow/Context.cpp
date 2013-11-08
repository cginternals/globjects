#include <cassert>

#include <GL/glew.h>

#include <glow/logging.h>
#include <glow/global.h>
#include <glow/Error.h>

#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..
#include <glowwindow/Context.h>


namespace glow
{

Context::Context()
:   m_swapInterval(VerticalSyncronization)
,   m_window(nullptr)
{
}

Context::~Context()
{
    release();
}

GLFWwindow * Context::window()
{
    return m_window;
}

bool Context::create(
    const ContextFormat & format
,   const int width
,   const int height)
{
    if (isValid())
    {
        warning() << "Context is already valid. Create was probably called before.";
        return false;
    }

    m_format = format;

    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, format.majorVersion());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, format.minorVersion());
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, format.profile() == ContextFormat::CoreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    
    glfwWindowHint(GLFW_DEPTH_BITS, format.depthBufferSize());
    glfwWindowHint(GLFW_STENCIL_BITS, format.stencilBufferSize());
    glfwWindowHint(GLFW_RED_BITS, format.redBufferSize());
    glfwWindowHint(GLFW_GREEN_BITS, format.greenBufferSize());
    glfwWindowHint(GLFW_BLUE_BITS, format.blueBufferSize());
    glfwWindowHint(GLFW_ALPHA_BITS, format.alphaBufferSize());
    glfwWindowHint(GLFW_SAMPLES, format.samples());

    m_window = glfwCreateWindow(width, height, "glow", nullptr, nullptr);

    if (!m_window)
    {
        release();
        return false;
    }

    makeCurrent();

    if (GLEW_OK != glewInit())
    {
        fatal() << "GLEW initialization failed (glewInit).";

        release();
        return false;
    }
    // NOTE: should be safe to ignore:
    // http://www.opengl.org/wiki/OpenGL_Loading_Library
    // http://stackoverflow.com/questions/10857335/opengl-glgeterror-returns-invalid-enum-after-call-to-glewinit
    Error::clear();

    doneCurrent();

    setSwapInterval();

    ContextFormat::verify(format, m_format);
    return true;
}

void Context::release()
{
    if (!isValid())
        return;

    glfwDestroyWindow(m_window);
    m_window = nullptr;
}

void Context::swap()
{
    if (!isValid())
        return;

    glfwSwapBuffers(m_window);
}

bool Context::isValid() const
{
	return m_window != nullptr;
}

const ContextFormat & Context::format() const
{
	return m_format;
}

const std::string Context::swapIntervalString(const SwapInterval swapInterval)
{
	switch(swapInterval)
	{
    case NoVerticalSyncronization:
        return "NoVerticalSyncronization";
    case VerticalSyncronization:
        return "VerticalSyncronization";
    case AdaptiveVerticalSyncronization:
        return "AdaptiveVerticalSyncronization";
    default:
        return "";
	};
}

Context::SwapInterval Context::swapInterval() const
{
	return m_swapInterval;
}

void Context::setSwapInterval(const SwapInterval interval)
{
	if (interval == m_swapInterval)
		return;

    m_swapInterval = interval;
    setSwapInterval();
}

void Context::setSwapInterval()
{
    makeCurrent();
    glfwSwapInterval(m_swapInterval);
    doneCurrent();
}

void Context::makeCurrent()
{
    if (!isValid())
        return;

    glfwMakeContextCurrent(m_window);
}

void Context::doneCurrent()
{
    if (!isValid())
        return;

    glfwMakeContextCurrent(0);
}

} // namespace glow
