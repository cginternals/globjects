
#pragma once

#include <functional>
#include <tuple>

#include <globjects/globjects_api.h>

#include <globjects/base/AbstractFunctionCall.h>


namespace globjects
{


template <typename... Arguments>
class FunctionCall : public AbstractFunctionCall
{
public:
    using FunctionPointer = void (*)(Arguments...);

    FunctionCall();
    FunctionCall(FunctionPointer function, Arguments... arguments);

    virtual void operator()() override;
    virtual void * identifier() const override;

protected:
    mutable FunctionPointer m_functionPointer;
    std::function<void(Arguments...)> m_function;
    std::tuple<Arguments...> m_arguments;
};


} // namespace globjects


#include <globjects/base/FunctionCall.inl>
