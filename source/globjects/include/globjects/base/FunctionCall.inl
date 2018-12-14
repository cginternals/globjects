
#pragma once


#include <cstddef>
#include <type_traits>
#include <utility>


namespace
{


// http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments

template<size_t N>
struct ApplyHelper
{
    template<typename F, typename T, typename... A>
    static inline auto apply(F && f, T && t, A &&... a)
        -> decltype(
            ApplyHelper<N-1>::apply(
                std::forward<F>(f),
                std::forward<T>(t),
                std::get<N-1>(std::forward<T>(t)),
                std::forward<A>(a)...
        )
    )
    {
        return ApplyHelper<N-1>::apply(
            std::forward<F>(f),
            std::forward<T>(t),
            std::get<N-1>(std::forward<T>(t)),
            std::forward<A>(a)...
        );
    }
};


template<>
struct ApplyHelper<0>
{
    template<typename F, typename T, typename... A>
    static inline auto apply(F && f, T &&, A &&... a)
        -> decltype(
            std::forward<F>(f)(std::forward<A>(a)...)
        )
    {
        return std::forward<F>(f)(
            std::forward<A>(a)...
        );
    }
};


template<typename F, typename T>
inline auto apply(F && f, T && t)
    -> decltype(
        ApplyHelper<std::tuple_size<typename std::decay<T>::type>::value>::apply(
            std::forward<F>(f),
            std::forward<T>(t)
        )
    )
{
    return ApplyHelper<std::tuple_size<typename std::decay<T>::type>::value>::apply(
        std::forward<F>(f),
        std::forward<T>(t)
    );
}


} // namespace


namespace globjects
{


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
void FunctionCall<Arguments...>::operator()()
{
    ::apply(m_function, m_arguments);
}

template <typename... Arguments>
void * FunctionCall<Arguments...>::identifier() const
{
    return *reinterpret_cast<void**>(&m_functionPointer);
}


} // namespace globjects
