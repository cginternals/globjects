
#include <cassert>

#include <GL/glew.h>

#ifdef WIN32
#include "WinContext.h"
#elif __APPLE__
#include "MacContext.h"
#else
#include "LinContext.h"
#endif

#include <glow/logging.h>
#include <glow/query.h>
#include <glow/Error.h>
#include <glow/Context.h>


namespace glow
{

Context::Context()
:   m_swapInterval(VerticalSyncronization)
,   m_context(nullptr)
{
#ifdef WIN32
    m_context = new WinContext();
#elif __APPLE__
    m_context = new MacContext();
#else
    m_context = new LinContext();
#endif
}

Context::~Context()
{
    if (isValid())
        release();

    delete m_context;
    m_context = nullptr;
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

    m_context->create(hWnd, m_format);

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

    ContextFormat::verify(format, m_format);

    return true;
}

void Context::release()
{
    if (!isValid())
        return;

    m_context->release();

    assert(NULL == m_context->id());
}

void Context::swap()
{
    if(FALSE == m_context->swap())
        warning() << "Swapping buffers failed (SwapBuffers). Error: " << GetLastError();
}

int Context::id() const
{
    return m_context->id();
}

bool Context::isValid() const
{
	return m_context->isValid();
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

	if (m_context->setSwapInterval(m_swapInterval))
        return true;

    CHECK_ERROR;
    warning() << "Setting swap interval to " << swapIntervalString(m_swapInterval) << " (" << m_swapInterval << ") failed. Error: " << GetLastError();

    return false;

    doneCurrent();
}

bool Context::makeCurrent()
{
    if (!isValid())
        return false;

    const BOOL result = m_context->makeCurrent();
    if (!result)
        fatal() << "Making the OpenGL context current failed (wglMakeCurrent). Error: " << GetLastError();

    return TRUE == result;
}

bool Context::doneCurrent()
{
    if (!isValid())
        return false;

    const BOOL result = m_context->doneCurrent();
    if (!result)
        warning() << "Release of RC failed (wglMakeCurrent). Error: " << GetLastError();

    return TRUE == result;
}

} // namespace glow
