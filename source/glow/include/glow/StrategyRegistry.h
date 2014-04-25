#pragma once


#include <glow/glow_api.h>

namespace glow
{

class AbstractUniformStrategy;
class AbstractBufferStrategy;

class GLOW_API StrategyRegistry
{
public:
    StrategyRegistry();
    ~StrategyRegistry();

    AbstractUniformStrategy & uniformStrategy();
    AbstractBufferStrategy & bufferStrategy();
protected:
    AbstractUniformStrategy * m_uniformStrategy;
    AbstractBufferStrategy * m_bufferStrategy;
};

} // namespace glow
