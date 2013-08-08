
#include <cassert>

#include <GL/glew.h>

#ifdef WIN32
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <GL/glx.h>
#include <GL/GLUx.h>
#endif

#include <glow/Log.h>
#include <glow/Error.h>
#include <glow/Context.h>


namespace glow
{

/** NOTE: These static inline functions are not part of the context interface,
    to provide identical interfaces over all supported platforms.
*/

static inline PIXELFORMATDESCRIPTOR pixelFormatDescriptor(const ContextFormat & format)
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
    ,   format.redBufferSize()          // BYTE  cRedBits
    ,   0                               // BYTE  cRedShift
    ,   format.greenBufferSize()        // BYTE  cGreenBits
    ,   0                               // BYTE  cGreenShift
    ,   format.blueBufferSize()         // BYTE  cBlueBits
    ,   0                               // BYTE  cBlueShift
    ,   format.alphaBufferSize()        // BYTE  cAlphaBits
    ,   0                               // BYTE  cAlphaShift
    ,   0                               // BYTE  cAccumBits
    ,   0                               // BYTE  cAccumRedBits
    ,   0                               // BYTE  cAccumGreenBits
    ,   0                               // BYTE  cAccumBlueBits
    ,   0                               // BYTE  cAccumAlphaBits
    ,   format.depthBufferSize()        // BYTE  cDepthBits
    ,   format.stencilBufferSize()      // BYTE  cStencilBits
    ,   0                               // BYTE  cAuxBuffers
    ,   PFD_MAIN_PLANE                  // BYTE  iLayerType
    ,   0                               // BYTE  bReserved
    ,   0                               // DWORD dwLayerMask
    ,   0                               // DWORD dwVisibleMask
    ,   0                               // DWORD dwDamageMask
    };

    return pfd;
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

    m_hWnd = hWnd;
    m_id = NULL;

    PIXELFORMATDESCRIPTOR pfd(pixelFormatDescriptor(format));

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

    // finally create ogl context

    m_hRC = wglCreateContext(m_hDC);
    if (NULL == m_hRC)
    {
        fatal() << "Creating OpenGL context failed (wglCreateContext). Error: " << GetLastError();
        release();
        return false;
    }

    if (makeCurrent())
    {
        release();
        return false;
    }

    m_id = reinterpret_cast<int>(m_hRC);

    return true;
}

void Context::release()
{
    if (!isValid())
        return;

    if (!wglMakeCurrent(NULL, NULL))
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

	bool result(false);

#ifdef WIN32

	typedef bool (WINAPI * SWAPINTERVALEXTPROC) (int) ;
	static SWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

	if (!wglSwapIntervalEXT)
		wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
	if (wglSwapIntervalEXT)
		result = wglSwapIntervalEXT(interval);

#elif __APPLE__

	qWarning("TODO: Setting swap interval is currently not implemented for __APPLE__");

#else

	typedef int (APIENTRY * SWAPINTERVALEXTPROC) (int) ;
	static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

	if (!glXSwapIntervalSGI)
	{
		const GLubyte * sgi = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
		glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(glXGetProcAddress(sgi));
	}
	if (glXSwapIntervalSGI)
		result = glXSwapIntervalSGI(interval);

#endif

	if (!result)
	{
		warning() << "Setting swap interval to " << swapIntervalString(interval) << " failed.";
        CHECK_ERROR;
	}
	else
		m_swapInterval = interval;

	return result;
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
