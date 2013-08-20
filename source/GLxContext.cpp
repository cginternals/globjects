
#ifdef __linux__

#include <cassert>

#include <GL/glew.h>

#include <GL/glxew.h>
#include <GL/glx.h>

#include <glow/logging.h>
#include <glow/query.h>
#include <glow/Error.h>

#include "GLxContext.h"


namespace glow
{

Display * GLxContext::s_display = nullptr;

GLxContext::GLxContext(Context & context)
:   AbstractNativeContext(context)
,   m_display(nullptr)
,   m_hWnd(0L)
,   m_context(nullptr)
,   m_id(-1)
{
}

GLxContext::~GLxContext()
{
    release();

    assert(nullptr == m_display);
    assert(0L == m_hWnd);
    assert(nullptr == m_context);
    assert(-1 == m_id);
}

Display * GLxContext::getOrOpenDisplay()
{
    if(s_display)
        return s_display;

    s_display = XOpenDisplay(NULL);
    if (nullptr == s_display)
    {
        int dummy; // this is stupid! if a parameters is not required passing nullptr does not work.

        if(!glXQueryExtension(s_display, &dummy, &dummy))
        {
            fatal() << "Cannot conntext to X server (XOpenDisplay).";
            return nullptr;
        }
    }
    return s_display;
}

void GLxContext::closeDisplay()
{
    if(!s_display)
        return;

    XCloseDisplay(s_display);
    s_display = nullptr;
}

// example: http://wili.cc/blog/ogl3-glx.html

bool GLxContext::create(
    const int hWnd
,   ContextFormat & format)
{
    assert(!isValid());

    m_hWnd = static_cast< ::Window>(hWnd);

    m_display = getOrOpenDisplay();
    if (nullptr == m_display)
        return false;

    const int screen = DefaultScreen(m_display);
    const ::Window root = DefaultRootWindow(m_display);

    int viAttribs[] = { GLX_RGBA, GLX_USE_GL, GLX_DEPTH_SIZE, format.depthBufferSize(), GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8  
        , (format.swapBehavior() == ContextFormat::DoubleBuffering ? GLX_DOUBLEBUFFER : None), None };

    XVisualInfo * vi = glXChooseVisual(m_display, screen, viAttribs);
    if (nullptr == vi)
    {
        fatal() << "Choosing a visual failed (glXChooseVisual).";
        return false;
    }

    // create temporary ogl context

    ::GLXContext tempContext = glXCreateContext(m_display, vi, None, GL_TRUE);
    if (NULL == tempContext)
    {
        fatal() << "Creating temporary OpenGL context failed (glXCreateContext).";
        return false;
    }

    // check for GLX_ARB_create_context extension

    if (!glXMakeCurrent(m_display, m_hWnd, tempContext))
    {
        fatal() << "Making temporary OpenGL context current failed (glXMakeCurrent).";

        glXDestroyContext(m_display, tempContext);
        return false;
    }

    // http://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_(C%2B%2B/Win)

    if (GLEW_OK != glewInit())
    {
        fatal() << "GLEW initialization failed (glewInit).";
        CheckGLError();

        glXDestroyContext(m_display, tempContext);
        return false;
    }

    if (!GLXEW_ARB_create_context)
    {
        fatal() << "Mandatory extension GLX_ARB_create_context not supported.";

        glXDestroyContext(m_display, tempContext);
        return false;
    }

    // NOTE: this assumes that the driver creates a "defaulted" context with
    // the highest available opengl version.
    format.setVersionFallback(query::majorVersion(), query::minorVersion());

    glXMakeCurrent(m_display, 0L, nullptr);
    glXDestroyContext(m_display, tempContext);


    if(fatalVersionDisclaimer(format.version()))
        return false;


    // create context

    const int attributes[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(format.majorVersion())
    ,   GLX_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(format.minorVersion())
    ,   GLX_CONTEXT_PROFILE_MASK_ARB,  format.profile() == ContextFormat::CoreProfile ?
            GLX_CONTEXT_CORE_PROFILE_BIT_ARB : GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
    ,   GLX_CONTEXT_FLAGS_ARB, 0, None
    };

    int elemc;
    GLXFBConfig * fbConfig = glXChooseFBConfig(m_display, screen, NULL, &elemc);
    if (!fbConfig)
    {
        fatal() << "Choosing a Frame Buffer configuration failed (glXChooseFBConfig)";
        return false;
    }
    
    m_context = glXCreateContextAttribsARB(m_display, fbConfig[0], NULL, true, attributes);
    if (NULL == m_context)
    {
        fatal() << "Creating OpenGL context with attributes failed (glXCreateContextAttribsARB).";
        
        return false;
    }

    if (!glXIsDirect(m_display, m_context))
        warning() << "Direct rendering is not enabled (glXIsDirect).";
	
    CheckGLError();
	
    m_id = glXGetContextIDEXT(m_context);
    
    CheckGLError();
    
    return true;
}

void GLxContext::release()
{
    //~ assert(isValid());
    if(m_context)
    {
        if(m_context == glXGetCurrentContext() && !glXMakeCurrent(m_display, 0L, nullptr))
            warning() << "Release of context failed (glXMakeCurrent).";

        glXDestroyContext(m_display, m_context);
    }

    m_display = nullptr;
    m_hWnd = 0L;
    m_context = nullptr;

    m_id = -1;
}

void GLxContext::swap() const
{
    assert(isValid());

    if(ContextFormat::SingleBuffering == format().swapBehavior())
        return;

    glXSwapBuffers(m_display, m_hWnd);
}

int GLxContext::id() const
{
    return m_id;
}

bool GLxContext::isValid() const
{
    return 0 < id();
}

bool GLxContext::setSwapInterval(Context::SwapInterval swapInterval) const
{
    glXSwapIntervalEXT(m_display, m_hWnd, swapInterval);
    if(CheckGLError())
        warning() << "Setting swap interval to " << Context::swapIntervalString(swapInterval)
            << " (" << swapInterval << ") failed.";

    return false;
}

bool GLxContext::makeCurrent() const
{
    const Bool result = glXMakeCurrent(m_display, m_hWnd, m_context);
    
    CheckGLError();
    
    if (!result)
        fatal() << "Making the OpenGL context current failed (glXMakeCurrent).";

    return True == result;
}

bool GLxContext::doneCurrent() const
{
    const Bool result = glXMakeCurrent(m_display, 0L, nullptr);
    
    CheckGLError();
    
    if (!result)
        warning() << "Release of RC failed (glXMakeCurrent).";

    return True == result;
}

} // namespace glow

#endif
