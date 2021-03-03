
#include <globjects/Resource.h>

#include <glbinding/gl/functions.h>

#include "registry/ImplementationRegistry.h"

#include "implementations/AbstractBufferImplementation.h"
#include "implementations/AbstractProgramImplementation.h"
#include "implementations/AbstractProgramPipelineImplementation.h"
#include "implementations/AbstractFramebufferImplementation.h"
#include "implementations/AbstractTextureImplementation.h"
#include "implementations/AbstractQueryImplementation.h"
#include "implementations/AbstractRenderbufferImplementation.h"
#include "implementations/AbstractSamplerImplementation.h"
#include "implementations/AbstractShaderImplementation.h"
#include "implementations/AbstractTransformFeedbackImplementation.h"
#include "implementations/AbstractVertexAttributeBindingImplementation.h"


using namespace gl;


namespace globjects 
{

AbstractResource::AbstractResource(const bool hasOwnership)
: m_hasOwnership(hasOwnership)
{
}

AbstractResource::~AbstractResource()
{
}

bool AbstractResource::hasOwnership() const
{
    return m_hasOwnership;
}


IDTrait::IDTrait(GLuint id)
: m_id(id)
{
}

IDTrait::~IDTrait()
{
}

GLuint IDTrait::id() const
{
    return m_id;
}


IDResource::IDResource(const GLuint id)
: AbstractResource(true)
, IDTrait(id)
{
}

IDResource::IDResource(const GLuint id, const bool hasOwnership)
: AbstractResource(hasOwnership)
, IDTrait(id)
{
}

bool IDResource::isExternal() const
{
    return false;
}


ExternalResource::ExternalResource(const GLuint id)
: IDResource(id, false)
{
}

bool ExternalResource::isExternal() const
{
    return true;
}


InvalidResource::InvalidResource()
: IDResource(0, false)
{
}

InvalidResource::~InvalidResource()
{
}


BufferResource::BufferResource()
: IDResource(ImplementationRegistry::current().bufferImplementation().create())
{
}

BufferResource::~BufferResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().bufferImplementation().destroy(id());
    }
}


FrameBufferObjectResource::FrameBufferObjectResource()
: IDResource(ImplementationRegistry::current().framebufferImplementation().create())
{
}

FrameBufferObjectResource::~FrameBufferObjectResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().framebufferImplementation().destroy(id());
    }
}


ProgramResource::ProgramResource()
    : IDResource(ImplementationRegistry::current().programImplementation().create())
{
}

ProgramResource::~ProgramResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().programImplementation().destroy(id());
    }
}


ProgramPipelineResource::ProgramPipelineResource()
: IDResource(ImplementationRegistry::current().programPipelineImplementation().create())
{
}

ProgramPipelineResource::~ProgramPipelineResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().programPipelineImplementation().destroy(id());
    }
}


QueryResource::QueryResource()
: IDResource(ImplementationRegistry::current().queryImplementation().create())
{
}

QueryResource::~QueryResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().queryImplementation().destroy(id());
    }
}


RenderBufferObjectResource::RenderBufferObjectResource()
: IDResource(ImplementationRegistry::current().renderbufferImplementation().create())
{
}

RenderBufferObjectResource::~RenderBufferObjectResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().renderbufferImplementation().destroy(id());
    }
}


SamplerResource::SamplerResource()
: IDResource(ImplementationRegistry::current().samplerImplementation().create())
{
}

SamplerResource::~SamplerResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().samplerImplementation().destroy(id());
    }
}


ShaderResource::ShaderResource(GLenum type)
: IDResource(ImplementationRegistry::current().shaderImplementation().create(type))
{
}

ShaderResource::~ShaderResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().shaderImplementation().destroy(id());
    }
}


TextureResource::TextureResource(GLenum target)
: IDResource(ImplementationRegistry::current().textureBindlessImplementation().create(target))
{
}

TextureResource::~TextureResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().textureBindlessImplementation().destroy(id());
    }
}


TransformFeedbackResource::TransformFeedbackResource()
: IDResource(ImplementationRegistry::current().transformfeedbackImplementation().create())
{
}

TransformFeedbackResource::~TransformFeedbackResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().transformfeedbackImplementation().destroy(id());
    }
}


VertexArrayObjectResource::VertexArrayObjectResource()
: IDResource(ImplementationRegistry::current().attributeImplementation().create())
{
}

VertexArrayObjectResource::~VertexArrayObjectResource()
{
    if (hasOwnership())
    {
        ImplementationRegistry::current().attributeImplementation().destroy(id());
    }
}


} // namespace globjects
