#pragma once

#include <globjects/globjects_api.h>


#include <globjects/Shader.h>
#include <globjects/Program.h>
#include <globjects/AbstractUniform.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/DebugMessage.h>
#include <globjects/Object.h>


namespace glo
{

class AbstractUniformImplementation;
class AbstractBufferImplementation;
class AbstractFramebufferImplementation;
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

    void initialize(AbstractUniform::BindlessImplementation impl);
    void initialize(Buffer::BindlessImplementation impl);
    void initialize(Framebuffer::BindlessImplementation impl);
    void initialize(DebugMessage::Implementation impl);
    void initialize(Program::BinaryImplementation impl);
    void initialize(Shader::IncludeImplementation impl);
    void initialize(Object::NameImplementation impl);

    static ImplementationRegistry & current();

    AbstractUniformImplementation & uniformImplementation();
    AbstractBufferImplementation & bufferImplementation();
    AbstractFramebufferImplementation & framebufferImplementation();
    AbstractDebugImplementation & debugImplementation();
    AbstractProgramBinaryImplementation & programBinaryImplementation();
    AbstractShadingLanguageIncludeImplementation & shadingLanguageIncludeImplementation();
    AbstractObjectNameImplementation & objectNameImplementation();

protected:
    AbstractUniformImplementation * m_uniformImplementation;
    AbstractBufferImplementation * m_bufferImplementation;
    AbstractFramebufferImplementation * m_framebufferImplementation;
    AbstractDebugImplementation * m_debugImplementation;
    AbstractProgramBinaryImplementation * m_programBinaryImplementation;
    AbstractShadingLanguageIncludeImplementation * m_shadingLanguageIncludeImplementation;
    AbstractObjectNameImplementation * m_objectNameImplementation;
};

} // namespace glo
