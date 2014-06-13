#pragma once

#include <glow/StateSetting.h>

#include <glowbase/FunctionCall.h>

namespace glow
{

template <typename... Arguments>
StateSetting::StateSetting(void (*function)(Arguments...), Arguments... arguments)
    : StateSetting(new FunctionCall<Arguments...>(function, arguments...))
{
}

} // namespace glow
