#pragma once

#include <globjects/AbstractState.h>

#include <globjects/StateSetting.h>

namespace glo {

template <typename... Arguments>
void AbstractState::set(void (*function)(Arguments...), Arguments... arguments)
{
    add(new StateSetting(function, arguments...));
}

} // namespace glo
