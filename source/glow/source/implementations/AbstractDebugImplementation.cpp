#include "AbstractDebugImplementation.h"

#include <glow/glow.h>

#include "DebugImplementation.h"
#include "FallbackDebugImplementation.h"

namespace glow {

AbstractDebugImplementation::AbstractDebugImplementation()
{
}

AbstractDebugImplementation::~AbstractDebugImplementation()
{
}

AbstractDebugImplementation * AbstractDebugImplementation::create()
{
    if (hasExtension(GLOW_KHR_debug))
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

void AbstractDebugImplementation::setCallback(DebugMessageCallback::Callback callback)
{
    m_messageCallback.clearCallbacks();
    m_messageCallback.addCallback(callback);
}

void AbstractDebugImplementation::addCallback(DebugMessageCallback::Callback callback)
{
    m_messageCallback.addCallback(callback);
}

} // namespace glow
