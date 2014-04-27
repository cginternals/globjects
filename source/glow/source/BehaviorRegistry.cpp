#include <glow/BehaviorRegistry.h>

#include "behaviors/AbstractUniformBehavior.h"
#include "behaviors/AbstractBufferBehavior.h"

namespace glow {

BehaviorRegistry::BehaviorRegistry()
: m_uniformBehavior(nullptr)
, m_bufferBehavior(nullptr)
{
}

BehaviorRegistry::~BehaviorRegistry()
{
}

AbstractUniformBehavior & BehaviorRegistry::uniformBehavior()
{
    if (!m_uniformBehavior)
    {
        m_uniformBehavior = AbstractUniformBehavior::create();
    }

    return *m_uniformBehavior;
}

AbstractBufferBehavior & BehaviorRegistry::bufferBehavior()
{
    if (!m_bufferBehavior)
    {
        m_bufferBehavior = AbstractBufferBehavior::create();
    }

    return *m_bufferBehavior;
}

} // namespace glow

