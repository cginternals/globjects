#pragma once

#include <glow/logging.h>

#include <cassert>

#include <glowbase/formatString.h>

namespace glow 
{

template <typename... Arguments> void info(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    info() << glowbase::formatString(format, arguments...);
}

template <typename... Arguments> void debug(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    debug() << glowbase::formatString(format, arguments...);
}

template <typename... Arguments> void warning(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    warning() << glowbase::formatString(format, arguments...);
}

template <typename... Arguments> void critical(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    critical() << glowbase::formatString(format, arguments...);
}

template <typename... Arguments> void fatal(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    fatal() << glowbase::formatString(format, arguments...);
}

} // namespace glow
