#pragma once


#include <glow/glow_api.h>

namespace glow
{

class AbstractUniformStrategy;

class GLOW_API StrategyRegistry
{
public:
    StrategyRegistry();
    ~StrategyRegistry();

    AbstractUniformStrategy & uniformStrategy();
protected:
    AbstractUniformStrategy * m_uniformStrategy;
};

} // namespace glow
