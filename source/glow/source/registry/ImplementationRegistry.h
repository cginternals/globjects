#pragma once

#include <glow/glow_api.h>

namespace glow
{

class AbstractUniformImplementation;
class AbstractBufferImplementation;
class AbstractFrameBufferImplementation;

class GLOW_API ImplementationRegistry
{
public:
    ImplementationRegistry();
    ~ImplementationRegistry();

    static ImplementationRegistry & current();

    AbstractUniformImplementation & uniformImplementation();
    AbstractBufferImplementation & bufferImplementation();
    AbstractFrameBufferImplementation & frameBufferImplementation();
protected:
    AbstractUniformImplementation * m_uniformImplementation;
    AbstractBufferImplementation * m_bufferImplementation;
    AbstractFrameBufferImplementation * m_frameBufferImplementation;
};

} // namespace glow
