#pragma once

#include <globjects/StateSetting.h>

#include <globjects/base/FunctionCall.h>

namespace glo
{

template <typename... Arguments>
StateSetting::StateSetting(void (*function)(Arguments...), Arguments... arguments)
    : StateSetting(new FunctionCall<Arguments...>(function, arguments...))
{
}

} // namespace glo
