
#pragma once

#include <globjects/Shader.h>
#include <globjects/Program.h>
#include <globjects/AbstractUniform.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/DebugMessage.h>
#include <globjects/Object.h>
#include <globjects/Texture.h>
#include <globjects/VertexArray.h>


namespace globjects
{


class AbstractUniformImplementation;
class AbstractBufferImplementation;
class AbstractFramebufferImplementation;
class AbstractDebugImplementation;
class AbstractProgramBinaryImplementation;
class AbstractShadingLanguageIncludeImplementation;
class AbstractObjectNameImplementation;
class AbstractVertexAttributeBindingImplementation;
class AbstractTextureImplementation;
class AbstractTextureStorageImplementation;
class AbstractTextureStorageMultisampleImplementation;


class ImplementationRegistry
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
    void initialize(VertexArray::AttributeImplementation impl);
    void initialize(Texture::BindlessImplementation impl);
    void initialize(Texture::StorageImplementation impl);
    void initialize(Texture::StorageMultisampleImplementation impl);

    static ImplementationRegistry & current();

    AbstractUniformImplementation & uniformImplementation();
    AbstractBufferImplementation & bufferImplementation();
    AbstractFramebufferImplementation & framebufferImplementation();
    AbstractDebugImplementation & debugImplementation();
    AbstractProgramBinaryImplementation & programBinaryImplementation();
    AbstractShadingLanguageIncludeImplementation & shadingLanguageIncludeImplementation();
    AbstractObjectNameImplementation & objectNameImplementation();
    AbstractVertexAttributeBindingImplementation & attributeImplementation();
    AbstractTextureImplementation & textureBindlessImplementation();
    AbstractTextureStorageImplementation & textureStorageImplementation();
    AbstractTextureStorageMultisampleImplementation & textureStorageMultisampleImplementation();


protected:
    AbstractUniformImplementation * m_uniformImplementation;
    AbstractBufferImplementation * m_bufferImplementation;
    AbstractFramebufferImplementation * m_framebufferImplementation;
    AbstractDebugImplementation * m_debugImplementation;
    AbstractProgramBinaryImplementation * m_programBinaryImplementation;
    AbstractShadingLanguageIncludeImplementation * m_shadingLanguageIncludeImplementation;
    AbstractObjectNameImplementation * m_objectNameImplementation;
    AbstractTextureImplementation * m_textureBindlessImplementation;
    AbstractTextureStorageImplementation * m_textureStorageImplementation;
    AbstractTextureStorageMultisampleImplementation * m_textureStorageMultisampleImplementation;
    AbstractVertexAttributeBindingImplementation * m_attributeImplementation;
};


} // namespace globjects
