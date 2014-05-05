#pragma once

#include <glowbase/FunctionCall.h>
#include <glow/StateSetting.h>

namespace glow
{

template <typename... Arguments>
StateSetting::StateSetting(void (*function)(Arguments...), Arguments... arguments)
    : StateSetting(new FunctionCall<Arguments...>(function, arguments...))
{
}

} // namespace glow
