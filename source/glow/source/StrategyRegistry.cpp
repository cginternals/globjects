#include <glow/StrategyRegistry.h>

#include "strategies/AbstractUniformStrategy.h"
#include "strategies/AbstractBufferStrategy.h"

namespace glow {

StrategyRegistry::StrategyRegistry()
: m_uniformStrategy(nullptr)
, m_bufferStrategy(nullptr)
{
}

StrategyRegistry::~StrategyRegistry()
{
}

AbstractUniformStrategy & StrategyRegistry::uniformStrategy()
{
    if (!m_uniformStrategy)
    {
        m_uniformStrategy = AbstractUniformStrategy::create();
    }

    return *m_uniformStrategy;
}

AbstractBufferStrategy & StrategyRegistry::bufferStrategy()
{
    if (!m_bufferStrategy)
    {
        m_bufferStrategy = AbstractBufferStrategy::create();
    }

    return *m_bufferStrategy;
}

} // namespace glow

