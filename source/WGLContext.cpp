
#ifdef WIN32

#include <cassert>

#include <GL/glew.h>

#include <windows.h>
#include <GL/wglew.h>
#include <GL/GL.h>

#include <glow/logging.h>
#include <glow/query.h>
#include <glow/Error.h>

#include "WGLContext.h"


namespace glow
{

WGLContext::WGLContext(Context & context)
:   AbstractNativeContext(context)
,   m_hWnd(NULL)
,   m_hDC (NULL)
,   m_hRC (NULL)
{
}

WGLContext::~WGLContext()
{
    release();

    assert(NULL == m_hWnd);
    assert(NULL == m_hDC);
    assert(NULL == m_hRC);
}

PIXELFORMATDESCRIPTOR WGLContext::toPixelFormatDescriptor(const ContextFormat & format)
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

void WGLContext::fromPixelFormatDescriptor(
    ContextFormat & format
,   const PIXELFORMATDESCRIPTOR & pfd)
{
    format.setSwapBehavior((pfd.dwFlags & PFD_DOUBLEBUFFER) ?
        ContextFormat::DoubleBuffering : ContextFormat::SingleBuffering);

    format.setRedBufferSize(pfd.cRedBits);
    format.setGreenBufferSize(pfd.cGreenBits);
    format.setBlueBufferSize(pfd.cBlueBits);
    format.setAlphaBufferSize(pfd.cAlphaBits);

    format.setDepthBufferSize(pfd.cDepthBits);
    format.setStencilBufferSize(pfd.cStencilBits);
}

bool WGLContext::create(
    const int hWnd
,   ContextFormat & format)
{
    assert(!isValid());

    m_hWnd = reinterpret_cast<HWND>(hWnd);

    // convert fomrat to native pixelformat

    PIXELFORMATDESCRIPTOR pfd(toPixelFormatDescriptor(format));

    m_hDC = GetDC(m_hWnd);
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
        CheckGLError();

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
    format.setVersionFallback(query::majorVersion(), query::minorVersion());

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);


    if(fatalVersionDisclaimer(format.version()))
        return false;


    // create context

    const int attributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, format.majorVersion()
    ,   WGL_CONTEXT_MINOR_VERSION_ARB, format.minorVersion()
    ,   WGL_CONTEXT_PROFILE_MASK_ARB,  format.profile() == ContextFormat::CoreProfile ? 
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

    return true;
}

void WGLContext::release()
{
    if (m_hRC)
    {
        if (m_hRC == wglGetCurrentContext() && !wglMakeCurrent(NULL, NULL))
            warning() << "Release of DC and RC failed (wglMakeCurrent). Error: " << GetLastError();

        if (m_hRC && !wglDeleteContext(m_hRC))
            warning() << "Deleting OpenGL context failed (wglDeleteContext). Error: " << GetLastError();
    }
    m_hRC  = NULL;

    if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))
        warning() << "Releasing device context failed (ReleaseDC). Error: " << GetLastError();

    m_hDC  = NULL;
    m_hWnd = NULL;
}

void WGLContext::swap() const
{
    assert(isValid());

    if(ContextFormat::SingleBuffering == format().swapBehavior())
        return;

    if(FALSE == SwapBuffers(m_hDC))
        warning() << "Swapping buffers failed (SwapBuffers). Error: " << GetLastError();
}

int WGLContext::id() const
{
    return reinterpret_cast<int>(m_hRC);
}

bool WGLContext::isValid() const
{
	return 0 < id();
}

bool WGLContext::setSwapInterval(Context::SwapInterval swapInterval) const
{
    if (TRUE == wglSwapIntervalEXT(swapInterval))
        return true;

    warning() << "Setting swap interval to " << Context::swapIntervalString(swapInterval) 
        << " (" << swapInterval << ") failed. Error: " << GetLastError();

	return false;
}

bool WGLContext::makeCurrent() const
{
    const BOOL result = wglMakeCurrent(m_hDC, m_hRC);

    if (!result)
        fatal() << "Making the OpenGL context current failed (wglMakeCurrent). Error: " << GetLastError();

    return TRUE == result;
}

bool WGLContext::doneCurrent() const
{
    const BOOL result = wglMakeCurrent(m_hDC, NULL);

    if (!result)
        warning() << "Release of RC failed (wglMakeCurrent). Error: " << GetLastError();

    return TRUE == result;
}

} // namespace glow

#endif
