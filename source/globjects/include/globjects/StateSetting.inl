
#pragma once


#include <globjects/base/FunctionCall.h>


namespace globjects
{


template <typename... Arguments>
StateSetting::StateSetting(void (*function)(Arguments...), Arguments... arguments)
    : StateSetting(FunctionCall<Arguments...>::create(function, arguments...))
{
}


} // namespace globjects
