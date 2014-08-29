#pragma once

#include <globjects/globjects_api.h>

namespace glo
{

class AbstractUniformImplementation;
class AbstractBufferImplementation;
class AbstractFrameBufferImplementation;
class AbstractDebugImplementation;
class AbstractProgramBinaryImplementation;
class AbstractShadingLanguageIncludeImplementation;
class AbstractObjectNameImplementation;

class GLOBJECTS_API ImplementationRegistry
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
    AbstractProgramBinaryImplementation & programBinaryImplementation();
    AbstractShadingLanguageIncludeImplementation & shadingLanguageIncludeImplementation();
    AbstractObjectNameImplementation & objectNameImplementation();

protected:
    AbstractUniformImplementation * m_uniformImplementation;
    AbstractBufferImplementation * m_bufferImplementation;
    AbstractFrameBufferImplementation * m_frameBufferImplementation;
    AbstractDebugImplementation * m_debugImplementation;
    AbstractProgramBinaryImplementation * m_programBinaryImplementation;
    AbstractShadingLanguageIncludeImplementation * m_shadingLanguageIncludeImplementation;
    AbstractObjectNameImplementation * m_objectNameImplementation;
};

} // namespace glo
