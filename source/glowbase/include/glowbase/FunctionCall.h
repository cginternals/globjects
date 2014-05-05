#pragma once

#include <functional>
#include <tuple>

#include <glowbase/glowbase_api.h>

namespace glow
{

class GLOWBASE_API AbstractFunctionCall
{
public:
    virtual void operator()() = 0;
    virtual void * identifier() const = 0;

    virtual ~AbstractFunctionCall() {}
};


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

} // namespace glowbase

#include <glowbase/FunctionCall.hpp>
