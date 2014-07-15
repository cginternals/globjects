#pragma once

#include <glow/glow_api.h>

#include <glowbase/LogMessageBuilder.h>


namespace glbinding
{
    class Version;
}

namespace glow 
{

GLOW_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glbinding::Version & vector);

} // namespace glow
