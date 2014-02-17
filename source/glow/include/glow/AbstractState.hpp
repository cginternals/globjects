#pragma once

#include <glow/AbstractState.h>

#include <glow/StateSetting.h>

namespace glow {

template <typename... Arguments>
void AbstractState::set(void (*function)(Arguments...), Arguments... arguments)
{
    add(new StateSetting(function, arguments...));
}

} // namespace glow
