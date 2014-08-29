#pragma once

#include <globjects/globjects_api.h>


#include <globjects/Shader.h>
#include <globjects/Program.h>
#include <globjects/AbstractUniform.h>
#include <globjects/Buffer.h>
#include <globjects/FrameBufferObject.h>
#include <globjects/DebugMessage.h>


namespace glo
{

class AbstractUniformImplementation;
class AbstractBufferImplementation;
class AbstractFramebufferImplementation;
class AbstractDebugImplementation;
class AbstractProgramBinaryImplementation;
class AbstractShadingLanguageIncludeImplementation;

class GLOBJECTS_API ImplementationRegistry
{
public:
    ImplementationRegistry();
    ~ImplementationRegistry();

    void initialize();

    void initialize(AbstractUniform::BindlessImplementation impl);
    void initialize(Buffer::BindlessImplementation impl);
    void initialize(FrameBufferObject::BindlessImplementation impl);
    void initialize(DebugMessage::Implementation impl);
    void initialize(Program::BinaryImplementation impl);
    void initialize(Shader::IncludeImplementation impl);

    static ImplementationRegistry & current();

    AbstractUniformImplementation & uniformImplementation();
    AbstractBufferImplementation & bufferImplementation();
    AbstractFramebufferImplementation & framebufferImplementation();
    AbstractDebugImplementation & debugImplementation();
    AbstractProgramBinaryImplementation & programBinaryImplementation();
    AbstractShadingLanguageIncludeImplementation & shadingLanguageIncludeImplementation();
protected:
    AbstractUniformImplementation * m_uniformImplementation;
    AbstractBufferImplementation * m_bufferImplementation;
    AbstractFramebufferImplementation * m_framebufferImplementation;
    AbstractDebugImplementation * m_debugImplementation;
    AbstractProgramBinaryImplementation * m_programBinaryImplementation;
    AbstractShadingLanguageIncludeImplementation * m_shadingLanguageIncludeImplementation;
};

} // namespace glo
