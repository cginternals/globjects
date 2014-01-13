#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

#include <glow/logging.h>

#include "DebugMessageCallback.h"

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

void DebugMessageCallback::operator()(const glow::DebugMessage & message)
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

void DebugMessageCallback::callCallbacks(const glow::DebugMessage & message)
{
    for (auto& callback: m_callbacks)
    {
        callback(message);
    }
}

void DebugMessageCallback::defaultAction(const glow::DebugMessage & message)
{
    if (message.type == GL_DEBUG_TYPE_ERROR_ARB)
    {
        handleError(message);
    }
    else
    {
        handleDebug(message);
    }
}

void DebugMessageCallback::handleDebug(const glow::DebugMessage & message)
{
    glow::debug() << message.toString();
}

void DebugMessageCallback::handleError(const glow::DebugMessage & message)
{
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
    throw std::runtime_error(message.toString());
#else
    fatal() << message.toString();
#endif
}
