#pragma once

#include <glow/FunctionCall.h>

namespace {
// http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
template<size_t ...S> struct seq {};
template<int N, size_t ...S> struct gens : gens<N-1, N-1, S...> {};
template<size_t ...S> struct gens<0, S...>{ typedef seq<S...> type; };
// helper
template<typename R, template<typename...> class Params, typename... Args, size_t... I>
R call_helper(std::function<R(Args...)> const&func, Params<Args...> const&params, seq<I...>)
{ return func(std::get<I>(params)...); }
// "return func(params...)"
template<typename R, template<typename...> class Params, typename... Args>
R call(std::function<R(Args...)> const&func, Params<Args...> const&params)
{ return call_helper(func,params, typename gens<sizeof...(Args)>::type()); }
}

namespace glow {

template <typename... Arguments>
FunctionCall<Arguments...>::FunctionCall(FunctionPointer function, Arguments... arguments)
: m_functionPointer(function)
, m_function(function)
, m_arguments(arguments...)
{
}

template <typename... Arguments>
FunctionCall<Arguments...>::FunctionCall()
{
}

template <typename... Arguments>
void FunctionCall<Arguments...>::operator()() {
    call(m_function, m_arguments);
}

template <typename... Arguments>
void * FunctionCall<Arguments...>::identifier() const
{
    return *reinterpret_cast<void**>(&m_functionPointer);
}

} // namespace glow
