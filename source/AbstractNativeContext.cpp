
#include <glow/ContextFormat.h>
#include <glow/Version.h>
#include <glow/logging.h>

#include "AbstractNativeContext.h"

namespace glow 
{

AbstractNativeContext::AbstractNativeContext(Context & context)
:   m_context(context)
{
}

AbstractNativeContext::~AbstractNativeContext()
{
}

const ContextFormat & AbstractNativeContext::format() const
{
    return m_context.format();
}

bool AbstractNativeContext::fatalVersionDisclaimer(const Version & version)
{
    // fatal version disclaimer ;)
    if(version >= Version(3, 2))
        return false;

    fatal() << "OpenGL Versions prior to 3.2 (" << version << " requested)"
        << " are not supported within glow. This might result in erroneous behaviour.";

    return true;
}

} // namespace glow
