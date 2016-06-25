
#pragma once


#include <globjects/StateSetting.h>


namespace globjects
{


template <typename... Arguments>
void AbstractState::set(void (*function)(Arguments...), Arguments... arguments)
{
    add(new StateSetting(function, arguments...));
}


} // namespace globjects
