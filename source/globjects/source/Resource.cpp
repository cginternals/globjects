
#include <globjects/Resource.h>

#include <glbinding/gl/functions.h>

#include "registry/ImplementationRegistry.h"

#include "implementations/AbstractBufferImplementation.h"
#include "implementations/AbstractProgramImplementation.h"
#include "implementations/AbstractFramebufferImplementation.h"
#include "implementations/AbstractTextureImplementation.h"
#include "implementations/AbstractVertexAttributeBindingImplementation.h"


using namespace gl;


namespace 
{


template <typename CreateObjectsFunction>
GLuint createObject(CreateObjectsFunction function)
{
    GLuint id;
    function(1, &id);
    return id;
}

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
: IDResource(createObject(glGenProgramPipelines))
{
}

ProgramPipelineResource::~ProgramPipelineResource()
{
    deleteObject(glDeleteProgramPipelines, id(), hasOwnership());
}


QueryResource::QueryResource()
: IDResource(createObject(glGenQueries))
{
}

QueryResource::~QueryResource()
{
    deleteObject(glDeleteQueries, id(), hasOwnership());
}


RenderBufferObjectResource::RenderBufferObjectResource()
: IDResource(createObject(glGenRenderbuffers))
{
}

RenderBufferObjectResource::~RenderBufferObjectResource()
{
    deleteObject(glDeleteRenderbuffers, id(), hasOwnership());
}


SamplerResource::SamplerResource()
: IDResource(createObject(glGenSamplers))
{
}

SamplerResource::~SamplerResource()
{
    deleteObject(glDeleteSamplers, id(), hasOwnership());
}

ShaderResource::ShaderResource(GLenum type)
: IDResource(glCreateShader(type))
{
}

ShaderResource::~ShaderResource()
{
    if (hasOwnership())
    {
        glDeleteShader(id());
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
: IDResource(createObject(glGenTransformFeedbacks))
{
}

TransformFeedbackResource::~TransformFeedbackResource()
{
    deleteObject(glDeleteTransformFeedbacks, id(), hasOwnership());
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
