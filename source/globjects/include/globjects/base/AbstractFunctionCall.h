#pragma once

#include <globjects/globjects_api.h>

namespace globjects
{

class GLOBJECTS_API AbstractFunctionCall
{
public:
    AbstractFunctionCall() = default;
    virtual ~AbstractFunctionCall() = default;

    virtual void operator()() = 0;
    virtual void * identifier() const = 0;
};

} // namespace globjects
