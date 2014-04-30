#include <glow/BehaviorRegistry.h>

#include "behaviors/AbstractUniformBehavior.h"
#include "behaviors/AbstractBufferBehavior.h"
#include "behaviors/AbstractFrameBufferBehavior.h"

namespace glow {

BehaviorRegistry::BehaviorRegistry()
: m_uniformBehavior(nullptr)
, m_bufferBehavior(nullptr)
, m_frameBufferBehavior(nullptr)
{
}

BehaviorRegistry::~BehaviorRegistry()
{
    delete m_uniformBehavior;
    delete m_bufferBehavior;
    delete m_frameBufferBehavior;
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

AbstractFrameBufferBehavior & BehaviorRegistry::frameBufferBehavior()
{
    if (!m_frameBufferBehavior)
    {
        m_frameBufferBehavior = AbstractFrameBufferBehavior::create();
    }

    return *m_frameBufferBehavior;
}

} // namespace glow

