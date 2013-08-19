
#include <glow/ContextFormat.h>

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

} // namespace glow
