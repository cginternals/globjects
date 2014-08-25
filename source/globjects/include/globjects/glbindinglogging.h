#pragma once

#include <globjects/globjects_api.h>

#include <globjects-base/LogMessageBuilder.h>


namespace glbinding
{
    class Version;
}

namespace glo 
{

GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glbinding::Version & vector);

} // namespace glo
