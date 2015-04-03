
#include <common/Context.h>

#include <cassert>
#include <map>

#include <glbinding/gl/gl.h>

#include <glbinding/ContextInfo.h>
#include <glbinding/ProcAddress.h>
#include <glbinding/Binding.h>
#include <glbinding/Version.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
// which requires APIENTRY in windows..

#include <globjects/globjects.h>
#include <globjects/base/baselogging.h>

#include <common/ContextFormat.h>


using namespace gl;
using namespace globjects;

glbinding::Version Context::maxSupportedVersion()
{
    glbinding::Version version;

    /* GLFW3 does not set default hint values on window creation so at least
    the default values must be set before glfwCreateWindow can be called.
    cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    */
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

#ifdef __APPLE__
    /*
    * Using OS X the following hints must be set for proper context initialization
    * (cf. http://stackoverflow.com/questions/19969937/getting-a-glsl-330-context-on-osx-10-9-mavericks)
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

    // create window for version check
    GLFWwindow * window = glfwCreateWindow(1, 1, "VersionCheck", nullptr, nullptr);

    if (window)
    {
        glfwMakeContextCurrent(window);

        glbinding::Binding::initialize(false);
        version = glbinding::ContextInfo::version();

        glfwMakeContextCurrent(nullptr);
        glfwDestroyWindow(window);
    }
    return version;
}

GLFWwindow * Context::create(
    const ContextFormat & format
,   bool verify
,   int width
,   int height
,   GLFWmonitor * monitor)
{
    glbinding::Version version = format.version();

    if (verify) // check if version is valid and supported
        version = ContextFormat::validateVersion(format.version(), maxSupportedVersion());

    /** GLFW3 does not set default hint values on window creation so at least
        the default values must be set before glfwCreateWindow can be called.
        cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    */
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, true);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.m_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.m_minor);

#ifdef __APPLE__

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#else

    if (version >= glbinding::Version(3, 0))
    {
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, format.forwardCompatible());
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, format.debugContext());
    }

    if (version >= glbinding::Version(3, 2))
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, format.profile() == ContextFormat::Profile::Core ? GLFW_OPENGL_CORE_PROFILE :
            (format.profile() == ContextFormat::Profile::Compatibility ? GLFW_OPENGL_COMPAT_PROFILE : GLFW_OPENGL_ANY_PROFILE));
    }

#endif

    glfwWindowHint(GLFW_DEPTH_BITS, format.depthBufferSize());
    glfwWindowHint(GLFW_STENCIL_BITS, format.stencilBufferSize());

    glfwWindowHint(GLFW_RED_BITS, format.redBufferSize());
    glfwWindowHint(GLFW_GREEN_BITS, format.greenBufferSize());
    glfwWindowHint(GLFW_BLUE_BITS, format.blueBufferSize());
    glfwWindowHint(GLFW_ALPHA_BITS, format.alphaBufferSize());

    glfwWindowHint(GLFW_STEREO, format.stereo());
    glfwWindowHint(GLFW_SAMPLES, format.samples());

    GLFWwindow * window = glfwCreateWindow(width, height, "", monitor, nullptr);

    if (window)
    {
        glfwMakeContextCurrent(window);
        glbinding::Binding::initialize(false);
        glfwMakeContextCurrent(nullptr);
    }
    return window;
}

const std::string & Context::swapIntervalString(const SwapInterval interval)
{
    static const std::map<SwapInterval, std::string> swapiIdentifier = {
        { SwapInterval::NoVerticalSyncronization, "NoVerticalSyncronization" }
      , { SwapInterval::VerticalSyncronization, "VerticalSyncronization" }
      , { SwapInterval::AdaptiveVerticalSyncronization, "AdaptiveVerticalSyncronization" } };

    return swapiIdentifier.at(interval);
}


Context::Context(GLFWwindow * window)
: m_swapInterval(SwapInterval::NoVerticalSyncronization)
, m_format(nullptr)
, m_window(window)
, m_handle(0)
{
    assert(m_window);

    GLFWwindow * current = glfwGetCurrentContext();
    if (current != m_window)
        glfwMakeContextCurrent(m_window);

    m_handle = tryFetchHandle();

    if (current != m_window)
        glfwMakeContextCurrent(current);
}

Context::~Context()
{
    delete m_format;
}

std::string Context::version()
{
    assert(0 != glbinding::getCurrentContext());
    return glbinding::ContextInfo::version().toString();
}

std::string Context::vendor()
{
    assert(0 != glbinding::getCurrentContext());
    return glbinding::ContextInfo::vendor();
}

std::string Context::renderer()
{
    assert(0 != glbinding::getCurrentContext());
    return glbinding::ContextInfo::renderer();
}

bool Context::isValid() const
{
    return 0 < handle();
}

glbinding::ContextHandle Context::tryFetchHandle()
{
    const glbinding::ContextHandle handle = glbinding::getCurrentContext();

    if (0 == handle)
        critical("Acquiring OpenGL context handle failed.");

    return handle;
}

Context::SwapInterval Context::swapInterval() const
{
    return m_swapInterval;
}

void Context::setSwapInterval(const SwapInterval interval)
{
    //if (interval == m_swapInterval) // initialized value might not match or explicit reset might be required
    //    return true;

    GLFWwindow * current = glfwGetCurrentContext();

    if (current != m_window)
        glfwMakeContextCurrent(m_window);

#ifdef WIN32

    using SWAPINTERVALEXTPROC = bool(*)(int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT(nullptr);

    bool result(false);
    if (!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(glbinding::getProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(static_cast<int>(interval));

    if(!result)
        warning("Setting swap interval to % failed.", swapIntervalString(interval));

#else

    glfwSwapInterval(static_cast<int>(interval));

#endif

    if (current != m_window)
        glfwMakeContextCurrent(current);

    m_swapInterval = interval;
}

glbinding::ContextHandle Context::handle() const
{
    return m_handle;
}

const ContextFormat & Context::format() const
{
    assert(isValid());

    if (m_format)
        return *m_format;

    // create and retrive format if not done already

    m_format = new ContextFormat();

    GLFWwindow * current = glfwGetCurrentContext();
    if (current != m_window)
        glfwMakeContextCurrent(m_window);
 
    m_format->setVersion(glbinding::ContextInfo::version());

    if (m_format->version() >= glbinding::Version(3, 2))
        m_format->setProfile(isCoreProfile() ? ContextFormat::Profile::Core : ContextFormat::Profile::Compatibility);
    else
        m_format->setProfile(ContextFormat::Profile::None);

    if (current != m_window)
        glfwMakeContextCurrent(current);


    //GLint i;
    //GLboolean b;

    //i = -1; glGetIntegerv(GLenum::GL_DEPTH_BITS, &i);
    //m_format->setDepthBufferSize(i);
    //i = -1; glGetIntegerv(GLenum::GL_STENCIL_BITS, &i);
    //m_format->setStencilBufferSize(i);
    //i = -1; glGetIntegerv(GLenum::GL_RED_BITS, &i);

    //m_format->setRedBufferSize(i);
    //i = -1; glGetIntegerv(GLenum::GL_GREEN_BITS, &i);
    //m_format->setGreenBufferSize(i);
    //i = -1; glGetIntegerv(GLenum::GL_BLUE_BITS, &i);
    //m_format->setBlueBufferSize(i);
    //i = -1; glGetIntegerv(GLenum::GL_ALPHA_BITS, &i);
    //m_format->setAlphaBufferSize(i);
    //i = -1; glGetIntegerv(GLenum::GL_SAMPLES, &i);
    //m_format->setSamples(i);
    //b = GL_FALSE;  glGetBooleanv(GLenum::GL_STEREO, &b);
    //m_format->setStereo(b == GL_TRUE);


    return *m_format;
}

void Context::makeCurrent() const
{
    if (m_window)
        glfwMakeContextCurrent(m_window);
}

void Context::doneCurrent() const
{
    if (m_window && m_window == glfwGetCurrentContext())
        glfwMakeContextCurrent(nullptr);
}
