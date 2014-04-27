#pragma once


#include <glow/glow_api.h>

namespace glow
{

class AbstractUniformBehavior;
class AbstractBufferBehavior;

class GLOW_API BehaviorRegistry
{
public:
    BehaviorRegistry();
    ~BehaviorRegistry();

    AbstractUniformBehavior & uniformBehavior();
    AbstractBufferBehavior & bufferBehavior();
protected:
    AbstractUniformBehavior * m_uniformBehavior;
    AbstractBufferBehavior * m_bufferBehavior;
};

} // namespace glow
