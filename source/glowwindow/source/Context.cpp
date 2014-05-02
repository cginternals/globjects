#include <glowwindow/Context.h>

#include <cassert>

#include <GL/glew.h>

#include <glow/logging.h>
#include <glow/glow.h>
#include <glow/Error.h>

#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..


using namespace glow;

namespace glowwindow
{

Context::Context()
: m_swapInterval(VerticalSyncronization)
, m_window(nullptr)
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

void Context::handleError(int errorCode, const char* errorMessage)
{
    glow::fatal("GLFW error 0x%x;: %;", errorCode, errorMessage);
}

bool Context::create(const ContextFormat & format, const int width, const int height, GLFWmonitor * monitor)
{
    if (isValid())
    {
        warning() << "Context is already valid. Create was probably called before.";
        return true;
    }

    if (!glfwInit())
    {
        fatal() << "Could not initialize GLFW.";
        return false;
    }

    glfwSetErrorCallback(&Context::handleError);

    m_format = format;
    prepareFormat(m_format);

    
    m_window = glfwCreateWindow(width, height, "glow", monitor, nullptr);

    if (!m_window)
    {
        fatal() << "Context creation failed (GLFW).";
        release();
        return false;
    }

    makeCurrent();

    if (!glow::init())
    {
        fatal() << "GLOW/GLEW initialization failed.";
        release();
        return false;
    }

    glfwSwapInterval(m_swapInterval);

    doneCurrent();

    // TODO: gather actual context format information and verify
    //ContextFormat::verify(format, m_format);

    return true;
}

void Context::prepareFormat(const ContextFormat & format)
{
    Version version = validateVersion(format.version());

    if (!format.version().isNull() && format.version() != version)
    {
        glow::warning() << "Changed unsupported OpenGL version from " << format.version() << " to " << version << ".";
    }
    
    /*
     * GLFW3 does not set default hint values on window creation so at least
     * the default values must be set before glfwCreateWindow can be called.
     * cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
     */
    glfwDefaultWindowHints();

#ifdef MAC_OS
  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
#else
  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minorVersion);

    if (version >= Version(3, 0))
    {
        if (format.forwardCompatible())
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        if (format.debugContext())
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,  GL_TRUE);
    }
  
    if (version >= Version(3, 2))
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE,
            format.profile() == ContextFormat::CoreProfile ? GLFW_OPENGL_CORE_PROFILE :
                (format.profile() == ContextFormat::CompatibilityProfile ? GLFW_OPENGL_COMPAT_PROFILE
                    : GLFW_OPENGL_ANY_PROFILE));
    }
  
#endif


    glfwWindowHint(GLFW_DEPTH_BITS, format.depthBufferSize());
    glfwWindowHint(GLFW_STENCIL_BITS, format.stencilBufferSize());
    glfwWindowHint(GLFW_RED_BITS, format.redBufferSize());
    glfwWindowHint(GLFW_GREEN_BITS, format.greenBufferSize());
    glfwWindowHint(GLFW_BLUE_BITS, format.blueBufferSize());
    glfwWindowHint(GLFW_ALPHA_BITS, format.alphaBufferSize());
    glfwWindowHint(GLFW_SAMPLES, format.samples());
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

Version Context::maximumSupportedVersion()
{
    Version maxVersion;
    
    /*
     * GLFW3 does not set default hint values on window creation so at least
     * the default values must be set before glfwCreateWindow can be called.
     * cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
     */
    glfwDefaultWindowHints();

#ifdef MAC_OS
    /*
     * Using OS X the following hints must be set for proper context initialization
     * (cf. http://stackoverflow.com/questions/19969937/getting-a-glsl-330-context-on-osx-10-9-mavericks)
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow * versionCheckWindow = glfwCreateWindow(1, 1, "VersionCheck", nullptr, nullptr);

    if (versionCheckWindow)
    {
        glfwMakeContextCurrent(versionCheckWindow);

        if (glow::init())
        {
            maxVersion = glow::Version::current();
        }

        glfwDestroyWindow(versionCheckWindow);
   }

    return maxVersion;
}

Version Context::validateVersion(const Version & version)
{
    Version maxVersion = maximumSupportedVersion();
    
    if (maxVersion.isNull())
    {
#ifdef MAC_OS
        maxVersion = Version(3, 2);
#else
        maxVersion = Version(3, 0);
#endif
      
    }

    if (version.isNull() || version > maxVersion)
    {
        return maxVersion;
    }

    if (!version.isValid())
    {
        Version nearestValidVersion = version.nearestValidVersion();
        if (nearestValidVersion > maxVersion)
        {
            return maxVersion;
        }

        return nearestValidVersion;
    }

    return version;
}

} // namespace glowwindow
