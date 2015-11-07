
#include "AbstractDebugImplementation.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include "DebugImplementation_DebugKHR.h"
#include "DebugImplementation_Legacy.h"

#ifdef GLOBJECTS_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif


using namespace gl;

namespace globjects 
{

DebugMessage::Callback AbstractDebugImplementation::s_defaultCallback = [](const DebugMessage & message) 
{
    if (message.type() == GL_DEBUG_TYPE_ERROR_ARB)
    {
#ifdef GLOBJECTS_GL_ERROR_RAISE_EXCEPTION
        throw std::runtime_error(message.toString());
#else
        fatal() << message.toString();
#endif
    }
    else if (message.type() != GL_DEBUG_TYPE_OTHER)
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

AbstractDebugImplementation * AbstractDebugImplementation::get(const DebugMessage::Implementation impl)
{
    if (impl == DebugMessage::Implementation::DebugKHR 
     && hasExtension(GLextension::GL_KHR_debug))
    {
        return DebugImplementation_DebugKHR::instance();
    }
    else
    {
        return DebugImplementation_Legacy::instance();
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

} // namespace globjects
