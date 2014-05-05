#pragma once

#include <glowbase/baselogging.h>

#include <cassert>

#include <glowbase/formatString.h>

namespace glow
{

template <typename... Arguments> void info(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    info() << formatString(format, arguments...);
}

template <typename... Arguments> void debug(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    debug() << formatString(format, arguments...);
}

template <typename... Arguments> void warning(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    warning() << formatString(format, arguments...);
}

template <typename... Arguments> void critical(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    critical() << formatString(format, arguments...);
}

template <typename... Arguments> void fatal(const char* format, Arguments... arguments)
{
    assert(format != nullptr);

    fatal() << formatString(format, arguments...);
}

} // namespace glowbase
