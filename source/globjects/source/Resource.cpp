#include "Resource.h"

#include <glbinding/gl/functions.h>

#include "registry/ImplementationRegistry.h"

#include "implementations/AbstractBufferImplementation.h"
#include "implementations/AbstractFramebufferImplementation.h"


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
void deleteObject(DeleteObjectsFunction function, GLuint id, bool hasOwnership)
{
    if (hasOwnership)
    {
        function(1, &id);
    }
}

}

namespace globjects 
{

AbstractResource::AbstractResource(bool hasOwnership)
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


IDResource::IDResource(GLuint id)
: AbstractResource(true)
, IDTrait(id)
{
}

IDResource::IDResource(GLuint id, bool hasOwnership)
: AbstractResource(hasOwnership)
, IDTrait(id)
{
}


ExternalResource::ExternalResource(GLuint id)
: IDResource(id, false)
{
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
        ImplementationRegistry::current().bufferImplementation().destroy(id());
}


FrameBufferObjectResource::FrameBufferObjectResource()
: IDResource(ImplementationRegistry::current().framebufferImplementation().create())
{
}

FrameBufferObjectResource::~FrameBufferObjectResource()
{
    if (hasOwnership())
        ImplementationRegistry::current().framebufferImplementation().destroy(id());
}


ProgramResource::ProgramResource()
: IDResource(glCreateProgram())
{
}

ProgramResource::~ProgramResource()
{
    if (hasOwnership())
    {
        glDeleteProgram(id());
    }
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


TextureResource::TextureResource()
: IDResource(createObject(glGenTextures))
{
}

TextureResource::~TextureResource()
{
    deleteObject(glDeleteTextures, id(), hasOwnership());
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
: IDResource(createObject(glGenVertexArrays))
{
}

VertexArrayObjectResource::~VertexArrayObjectResource()
{
    deleteObject(glDeleteVertexArrays, id(), hasOwnership());
}

} // namespace globjects
