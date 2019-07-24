
#include <globjects/Resource.h>

#include <glbinding/gl/functions.h>

#include "registry/ImplementationRegistry.h"

#include "implementations/AbstractBufferImplementation.h"
#include "implementations/AbstractProgramImplementation.h"
#include "implementations/AbstractProgramPipelineImplementation.h"
#include "implementations/AbstractFramebufferImplementation.h"
#include "implementations/AbstractTextureImplementation.h"
#include "implementations/AbstractQueryImplementation.h"
#include "implementations/AbstractVertexAttributeBindingImplementation.h"


using namespace gl;


namespace 
{


// TODO: remove after all code is used within implementations
template <typename CreateObjectsFunction>
GLuint createObject(CreateObjectsFunction function)
{
    GLuint id;
    function(1, &id);
    return id;
}

// TODO: remove after all code is used within implementations
template <typename DeleteObjectsFunction>
void deleteObject(DeleteObjectsFunction function, const GLuint id, const bool hasOwnership)
{
    if (!hasOwnership)
    {
        return;
    }

    function(1, &id);
}

}

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
: IDResource(createObject(glGenRenderbuffers)) // TODO: convert to implementation
{
}

RenderBufferObjectResource::~RenderBufferObjectResource()
{
    deleteObject(glDeleteRenderbuffers, id(), hasOwnership()); // TODO: convert to implementation
}


SamplerResource::SamplerResource()
: IDResource(createObject(glGenSamplers)) // TODO: convert to implementation
{
}

SamplerResource::~SamplerResource()
{
    deleteObject(glDeleteSamplers, id(), hasOwnership()); // TODO: convert to implementation
}


ShaderResource::ShaderResource(GLenum type)
: IDResource(glCreateShader(type)) // TODO: convert to implementation
{
}

ShaderResource::~ShaderResource()
{
    if (hasOwnership())
    {
        glDeleteShader(id()); // TODO: convert to implementation
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
: IDResource(createObject(glGenTransformFeedbacks)) // TODO: convert to implementation
{
}

TransformFeedbackResource::~TransformFeedbackResource()
{
    deleteObject(glDeleteTransformFeedbacks, id(), hasOwnership()); // TODO: convert to implementation
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
