#pragma once

#include <glow/glow_api.h>

namespace glow
{

class AbstractUniformImplementation;
class AbstractBufferImplementation;
class AbstractFrameBufferImplementation;
class AbstractDebugImplementation;

class GLOW_API ImplementationRegistry
{
public:
    ImplementationRegistry();
    ~ImplementationRegistry();

    void initialize();

    static ImplementationRegistry & current();

    AbstractUniformImplementation & uniformImplementation();
    AbstractBufferImplementation & bufferImplementation();
    AbstractFrameBufferImplementation & frameBufferImplementation();
    AbstractDebugImplementation & debugImplementation();
protected:
    AbstractUniformImplementation * m_uniformImplementation;
    AbstractBufferImplementation * m_bufferImplementation;
    AbstractFrameBufferImplementation * m_frameBufferImplementation;
    AbstractDebugImplementation * m_debugImplementation;
};

} // namespace glow
