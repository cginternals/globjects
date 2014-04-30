#pragma once


#include <glow/glow_api.h>

namespace glow
{

class AbstractUniformBehavior;
class AbstractBufferBehavior;
class AbstractFrameBufferBehavior;

class GLOW_API BehaviorRegistry
{
public:
    BehaviorRegistry();
    ~BehaviorRegistry();

    AbstractUniformBehavior & uniformBehavior();
    AbstractBufferBehavior & bufferBehavior();
    AbstractFrameBufferBehavior & frameBufferBehavior();
protected:
    AbstractUniformBehavior * m_uniformBehavior;
    AbstractBufferBehavior * m_bufferBehavior;
    AbstractFrameBufferBehavior * m_frameBufferBehavior;
};

} // namespace glow
