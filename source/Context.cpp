
#include <cassert>

#include <GL/glew.h>

#ifdef WIN32
#include <windows.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#else
#include <GL/glxew.h>
#include <GL/glx.h>
#include <GL/GLUx.h>
#endif

#include <glow/logging.h>
#include <glow/query.h>
#include <glow/Error.h>
#include <glow/Context.h>


namespace glow
{

/** NOTE: These static inline functions are not part of the context interface,
    to provide identical interfaces over all supported platforms.
*/

static inline PIXELFORMATDESCRIPTOR toPixelFormatDescriptor(const ContextFormat & format)
{
    // NTOE: TrippleBufferig not supported yet.
    // NOTE: Accumulation buffer is not supported.

    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd368826(v=vs.85).aspx
    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR)   // WORD  nSize
    ,   1                               // WORD  nVersion
    ,   PFD_DRAW_TO_WINDOW              // DWORD dwFlags
      | PFD_SUPPORT_OPENGL
      | (format.swapBehavior() == ContextFormat::DoubleBuffering ? PFD_DOUBLEBUFFER : NULL)
    ,   PFD_TYPE_RGBA                   // BYTE  iPixelType
    ,   32                              // BYTE  cColorBits;
    ,   0, 0, 0, 0, 0, 0                //       Not used
    ,   format.alphaBufferSize()        // BYTE  cAlphaBits
    ,   0, 0, 0, 0, 0, 0                //       Not used
    ,   format.depthBufferSize()        // BYTE  cDepthBits
    ,   format.stencilBufferSize()      // BYTE  cStencilBits
    ,   0                               // BYTE  cAuxBuffers
    ,   PFD_MAIN_PLANE                  // BYTE  iLayerType
    ,   0, 0, 0, 0                      //       Not used
    };

    return pfd;
}

static void inline fromPixelFormatDescriptor(
    ContextFormat & format
,   const PIXELFORMATDESCRIPTOR & pfd)
{
    format.setSwapBehavior((pfd.dwFlags & PFD_DOUBLEBUFFER) ? ContextFormat::DoubleBuffering : ContextFormat::SingleBuffering);

    format.setRedBufferSize(pfd.cRedBits);
    format.setGreenBufferSize(pfd.cGreenBits);
    format.setBlueBufferSize(pfd.cBlueBits);
    format.setAlphaBufferSize(pfd.cAlphaBits);

    format.setDepthBufferSize(pfd.cDepthBits);
    format.setStencilBufferSize(pfd.cStencilBits);
}

Context::Context()
:   m_swapInterval(VerticalSyncronization)
,   m_id(0)
,   m_hWnd(0)
,   m_hRC(0)
,   m_hDC(0)
{
}

Context::~Context()
{
    if (isValid())
        release();
}

bool Context::create(
    const int hWnd
,   const ContextFormat & format)
{
    if (isValid())
    {
        warning() << "Context is already valid. Create was probably called before.";
        return false;
    }

    m_format = format;

    m_hWnd = hWnd;
    m_id = NULL;

    // convert fomrat to native pixelformat

    PIXELFORMATDESCRIPTOR pfd(toPixelFormatDescriptor(m_format));

    m_hDC = GetDC(reinterpret_cast<HWND>(m_hWnd));
    if (NULL == m_hDC)
    {
        fatal() << "Obtaining a device context failed (GetDC). Error: " << GetLastError();
        release();
        return false;
    }

    int iPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
    if (0 == iPixelFormat)
    {
        fatal() << "Choosing a suitable pixelformat failed (ChoosePixelFormat). Error: " << GetLastError();
        release();
        return false;
    }

    if (FALSE == SetPixelFormat(m_hDC, iPixelFormat, &pfd))
    {
        fatal() << "Setting pixel format failed (SetPixelFormat). Error: " << GetLastError();
        release();
        return false;
    }

    // create temporary ogl context

    HGLRC tempRC = wglCreateContext(m_hDC);

    if (NULL == tempRC)
    {
        fatal() << "Creating temporary OpenGL context failed (wglCreateContext). Error: " << GetLastError();
        release();
        return false;
    }

    // check for WGL_ARB_create_context extension

    if (!wglMakeCurrent(m_hDC, tempRC))
    {
        fatal() << "Making temporary OpenGL context current failed (wglMakeCurrent). Error: " << GetLastError();
        release();
        return false;
    }

    // http://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_(C%2B%2B/Win)

    if (GLEW_OK != glewInit())
    {
        fatal() << "GLEW initialization failed (glewInit).";
        CHECK_ERROR;

        release();
        return false;
    }

    if (!WGLEW_ARB_create_context)
    {
        fatal() << "Mandatory extension WGL_ARB_create_context not supported.";
        release();
        return false;
    }
    // NOTE: this assumes that the driver creates a "defaulted" context with
    // the highest available opengl version.
    m_format.setVersionFallback(query::majorVersion(), query::minorVersion());

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);

    const int attributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, m_format.majorVersion()
    ,   WGL_CONTEXT_MINOR_VERSION_ARB, m_format.minorVersion()
    ,   WGL_CONTEXT_PROFILE_MASK_ARB,  m_format.profile() == ContextFormat::CoreProfile ? 
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
    ,   WGL_CONTEXT_FLAGS_ARB, 0, 0
    };

    m_hRC = wglCreateContextAttribsARB(m_hDC, 0, attributes);
    if (NULL == m_hRC)
    {
        fatal() << "Creating OpenGL context with attributes failed (wglCreateContextAttribsARB). Error: " << GetLastError();
        release();
        return false;
    }

    m_id = reinterpret_cast<int>(m_hRC);

    makeCurrent();

    if (GLEW_OK != glewInit())
    {
        fatal() << "GLEW initialization failed (glewInit).";
        CHECK_ERROR;

        release();
        return false;
    }

    doneCurrent();

    setSwapInterval();

    // convert native pixelformat back to format for format verification

    fromPixelFormatDescriptor(m_format, pfd);
    ContextFormat::verify(format, m_format);

    return true;
}

void Context::release()
{
    if (!isValid())
        return;

    if(m_hRC == wglGetCurrentContext() && !wglMakeCurrent(NULL, NULL))
        warning() << "Release of DC and RC failed (wglMakeCurrent). Error: " << GetLastError();

    if (m_hRC && !wglDeleteContext(m_hRC))
        warning() << "Deleting OpenGL context failed (wglDeleteContext). Error: " << GetLastError();

    m_hRC = NULL;
    m_id = 0;

    HWND hWnd = reinterpret_cast<HWND>(m_hWnd);
    if (m_hDC && !ReleaseDC(hWnd, m_hDC))
        warning() << "Releasing device context failed (ReleaseDC). Error: " << GetLastError();

    m_hDC = NULL;
}

void Context::swap()
{
    assert(isValid());

    if(FALSE == SwapBuffers(m_hDC))
        warning() << "Swapping buffers failed (SwapBuffers). Error: " << GetLastError();
}

int Context::id() const
{
    return m_id;
}

bool Context::isValid() const
{
	return 0 < id();
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

bool Context::setSwapInterval(const SwapInterval interval)
{
	if (interval == m_swapInterval)
		return true;

    const SwapInterval backup(m_swapInterval);
    m_swapInterval = interval;

    bool result = setSwapInterval();

    if (!result)
        m_swapInterval = backup;

    return result;		
}

bool Context::setSwapInterval()
{
    makeCurrent();

#ifdef WIN32

	if (wglSwapIntervalEXT(m_swapInterval))
        return true;

    CHECK_ERROR;
    warning() << "Setting swap interval to " << swapIntervalString(m_swapInterval) << " (" << m_swapInterval << ") failed. Error: " << GetLastError();

    return false;

#elif __APPLE__

#else

	//typedef int (APIENTRY * SWAPINTERVALEXTPROC) (int) ;
	//static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

	//if (!glXSwapIntervalSGI)
	//{
	//	const GLubyte * sgi = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
	//	glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(glXGetProcAddress(sgi));
	//}
	//if (glXSwapIntervalSGI)
	//	result = glXSwapIntervalSGI(m_swapInterval);

#endif

    doneCurrent();
}

bool Context::makeCurrent()
{
    if (!isValid())
        return false;

    const BOOL result = wglMakeCurrent(m_hDC, m_hRC);
    if (!result)
        fatal() << "Making the OpenGL context current failed (wglMakeCurrent). Error: " << GetLastError();

    return TRUE == result;
}

bool Context::doneCurrent()
{
    if (!isValid())
        return false;

    const BOOL result = wglMakeCurrent(m_hDC, NULL);
    if (!result)
        warning() << "Release of RC failed (wglMakeCurrent). Error: " << GetLastError();

    return TRUE == result;
}

} // namespace glow
