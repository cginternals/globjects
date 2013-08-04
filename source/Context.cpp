
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

Context::Context()
:   m_swapInterval(VerticalSyncronization)
,   m_id(0)
{
}

Context::~Context()
{
}

GLuint Context::id() const
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
		warning() << "Setting swap interval to " << interval << " failed.";
        CHECK_ERROR;
	}
	else
		m_swapInterval = interval;

	return result;
}

} // namespace glow
