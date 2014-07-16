#include "Resource.h"

#include <glbinding/gl/functions.h>

namespace {

template <typename CreateObjectsFunction>
gl::GLuint createObject(CreateObjectsFunction function)
{
    gl::GLuint id;
    function(1, &id);
    return id;
}

template <typename DeleteObjectsFunction>
void deleteObject(DeleteObjectsFunction function, gl::GLuint id, bool hasOwnership)
{
    if (hasOwnership)
    {
        function(1, &id);
    }
}

}

namespace glow {

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

IDTrait::IDTrait(gl::GLuint id)
: m_id(id)
{
}

IDTrait::~IDTrait()
{
}

gl::GLuint IDTrait::id() const
{
    return m_id;
}

IDResource::IDResource(gl::GLuint id)
: AbstractResource(true)
, IDTrait(id)
{
}

IDResource::IDResource(gl::GLuint id, bool hasOwnership)
: AbstractResource(hasOwnership)
, IDTrait(id)
{
}

ExternalResource::ExternalResource(gl::GLuint id)
: IDResource(id, false)
{
}

//============================================================

BufferResource::BufferResource()
: IDResource(createObject(gl::glGenBuffers))
{
}

BufferResource::~BufferResource()
{
    deleteObject(gl::glDeleteBuffers, id(), hasOwnership());
}

FrameBufferObjectResource::FrameBufferObjectResource()
: IDResource(createObject(gl::glGenFramebuffers))
{
}

FrameBufferObjectResource::~FrameBufferObjectResource()
{
    deleteObject(gl::glDeleteFramebuffers, id(), hasOwnership());
}

ProgramResource::ProgramResource()
: IDResource(gl::glCreateProgram())
{
}

ProgramResource::~ProgramResource()
{
    if (hasOwnership())
    {
        gl::glDeleteProgram(id());
    }
}

QueryResource::QueryResource()
: IDResource(createObject(gl::glGenQueries))
{
}

QueryResource::~QueryResource()
{
    deleteObject(gl::glDeleteQueries, id(), hasOwnership());
}

RenderBufferObjectResource::RenderBufferObjectResource()
: IDResource(createObject(gl::glGenRenderbuffers))
{
}

RenderBufferObjectResource::~RenderBufferObjectResource()
{
    deleteObject(gl::glDeleteRenderbuffers, id(), hasOwnership());
}

SamplerResource::SamplerResource()
: IDResource(createObject(gl::glGenSamplers))
{
}

SamplerResource::~SamplerResource()
{
    deleteObject(gl::glDeleteSamplers, id(), hasOwnership());
}

ShaderResource::ShaderResource(gl::GLenum type)
: IDResource(gl::glCreateShader(type))
{
}

ShaderResource::~ShaderResource()
{
    if (hasOwnership())
    {
        gl::glDeleteShader(id());
    }
}

TextureResource::TextureResource()
: IDResource(createObject(gl::glGenTextures))
{
}

TextureResource::~TextureResource()
{
    deleteObject(gl::glDeleteTextures, id(), hasOwnership());
}

TransformFeedbackResource::TransformFeedbackResource()
: IDResource(createObject(gl::glGenTransformFeedbacks))
{
}

TransformFeedbackResource::~TransformFeedbackResource()
{
    deleteObject(gl::glDeleteTransformFeedbacks, id(), hasOwnership());
}

VertexArrayObjectResource::VertexArrayObjectResource()
: IDResource(createObject(gl::glGenVertexArrays))
{
}

VertexArrayObjectResource::~VertexArrayObjectResource()
{
    deleteObject(gl::glDeleteVertexArrays, id(), hasOwnership());
}

} // namespace glow
