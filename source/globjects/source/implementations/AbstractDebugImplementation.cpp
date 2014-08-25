#include "AbstractDebugImplementation.h"

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include <glbinding/gl/enum.h>

#include "DebugImplementation.h"
#include "FallbackDebugImplementation.h"

#ifdef GLOBJECTS_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

namespace glo {

DebugMessage::Callback AbstractDebugImplementation::s_defaultCallback = [](const DebugMessage & message) {
    if (message.type() == gl::GL_DEBUG_TYPE_ERROR_ARB)
    {
#ifdef GLOBJECTS_GL_ERROR_RAISE_EXCEPTION
        throw std::runtime_error(message.toString());
#else
        fatal() << message.toString();
#endif
    }
    else
    {
        debug() << message.toString();
    }
};

AbstractDebugImplementation::AbstractDebugImplementation()
: m_messageCallback(s_defaultCallback)
{
}

AbstractDebugImplementation::~AbstractDebugImplementation()
{
}

AbstractDebugImplementation * AbstractDebugImplementation::create()
{
    if (hasExtension(gl::GLextension::GL_KHR_debug))
    {
        return new DebugImplementation();
    }
    else
    {
        return new FallbackDebugImplementation();
    }
}

bool AbstractDebugImplementation::isFallback()
{
    return false;
}

void AbstractDebugImplementation::setCallback(DebugMessage::Callback callback)
{
    m_messageCallback = callback;
}

void AbstractDebugImplementation::handleMessage(const DebugMessage & message) const
{
    m_messageCallback(message);
}

} // namespace glo
