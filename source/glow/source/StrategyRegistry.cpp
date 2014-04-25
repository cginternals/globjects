#include <glow/StrategyRegistry.h>

#include "strategies/AbstractUniformStrategy.h"

namespace glow {

StrategyRegistry::StrategyRegistry()
: m_uniformStrategy(nullptr)
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

} // namespace glow

