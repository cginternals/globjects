#include "DebugMessageCallback.h"

#include <glbinding/constants.h>

#include <glow/DebugMessage.h>

#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

#include <glow/logging.h>

namespace glow
{

DebugMessageCallback::DebugMessageCallback()
: m_registered(false)
{
}

bool DebugMessageCallback::isRegistered() const
{
    return m_registered;
}

void DebugMessageCallback::setRegistered(bool registered)
{
    m_registered = registered;
}

void DebugMessageCallback::operator()(const DebugMessage & message) const
{
    if (m_callbacks.empty())
    {
        defaultAction(message);
    }
    else
    {
        callCallbacks(message);
    }
}

void DebugMessageCallback::addCallback(Callback callback)
{
    m_callbacks.push_back(callback);
}

void DebugMessageCallback::clearCallbacks()
{
    m_callbacks.clear();
}

void DebugMessageCallback::callCallbacks(const DebugMessage & message) const
{
    for (auto & callback: m_callbacks)
    {
        callback(message);
    }
}

void DebugMessageCallback::defaultAction(const DebugMessage & message) const
{
    if (message.type() == gl::DEBUG_TYPE_ERROR_ARB)
    {
        handleError(message);
    }
    else
    {
        handleDebug(message);
    }
}

void DebugMessageCallback::handleDebug(const DebugMessage & message) const
{
    debug() << message.toString();
}

void DebugMessageCallback::handleError(const DebugMessage & message) const
{
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
    throw std::runtime_error(message.toString());
#else
    fatal() << message.toString();
#endif
}

} // namespace glow
