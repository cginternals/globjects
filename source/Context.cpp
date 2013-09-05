
#include <cassert>

#include <GL/glew.h>

#ifdef WIN32
#include "WGLContext.h"
#elif __APPLE__

#else
#include "GLxContext.h"
#endif

#include <glow/logging.h>
#include <glow/global.h>
#include <glow/Error.h>
#include <glow/Context.h>


namespace glow
{

Context::Context()
:   m_swapInterval(VerticalSyncronization)
,   m_context(nullptr)
{
#ifdef WIN32
    m_context = new WGLContext(*this);
#elif __APPLE__

#else
    m_context = new GLxContext(*this);
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

    if (!m_context->create(hWnd, m_format))
    {
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

    m_context->release();

    assert(-1 == m_context->id());
}

void Context::swap()
{
	if (isValid())
		m_context->swap();
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
	const bool result = m_context->setSwapInterval(m_swapInterval);
    doneCurrent();

    return result;
}

bool Context::makeCurrent()
{
    if (!isValid())
        return false;

    return m_context->makeCurrent();
}

bool Context::doneCurrent()
{
    if (!isValid())
        return false;

    return m_context->doneCurrent();
}

} // namespace glow
