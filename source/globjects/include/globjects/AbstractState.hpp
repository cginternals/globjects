#pragma once

#include <globjects/AbstractState.h>

#include <globjects/StateSetting.h>

namespace globjects {

template <typename... Arguments>
void AbstractState::set(void (*function)(Arguments...), Arguments... arguments)
{
    add(new StateSetting(function, arguments...));
}

} // namespace globjects
