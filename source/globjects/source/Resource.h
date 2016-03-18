#pragma once

#include <glbinding/gl/types.h>

namespace globjects 
{

class AbstractResource
{
public:
    AbstractResource(bool hasOwnership);
    virtual ~AbstractResource();

    bool hasOwnership() const;

protected:
    bool m_hasOwnership;
};


class IDTrait
{
public:
    IDTrait(gl::GLuint id);
    virtual ~IDTrait();

    gl::GLuint id() const;

protected:
    gl::GLuint m_id;
};


class IDResource : public AbstractResource, public IDTrait
{
public:
    IDResource(gl::GLuint id);

protected:
    IDResource(gl::GLuint id, bool hasOwnership);
};


class ExternalResource : public IDResource
{
public:
    ExternalResource(gl::GLuint id);
};


class InvalidResource : public IDResource
{
public:
    InvalidResource();
    ~InvalidResource();
};


class BufferResource : public IDResource
{
public:
    BufferResource();
    ~BufferResource();
};


class FrameBufferObjectResource : public IDResource
{
public:
    FrameBufferObjectResource();
    ~FrameBufferObjectResource();
};


class ProgramResource : public IDResource
{
public:
    ProgramResource();
    ~ProgramResource();
};


class ProgramPipelineResource : public IDResource
{
public:
    ProgramPipelineResource();
    ~ProgramPipelineResource();
};


class QueryResource : public IDResource
{
public:
    QueryResource();
    ~QueryResource();
};


class RenderBufferObjectResource : public IDResource
{
public:
    RenderBufferObjectResource();
    ~RenderBufferObjectResource();
};


class SamplerResource : public IDResource
{
public:
    SamplerResource();
    ~SamplerResource();
};


class ShaderResource : public IDResource
{
public:
    ShaderResource(gl::GLenum type);
    ~ShaderResource();
};


class TextureResource : public IDResource
{
public:
    TextureResource(gl::GLenum target);
    ~TextureResource();
};


class TransformFeedbackResource : public IDResource
{
public:
    TransformFeedbackResource();
    ~TransformFeedbackResource();
};


class VertexArrayObjectResource : public IDResource
{
public:
    VertexArrayObjectResource();
    ~VertexArrayObjectResource();
};

} // namespace globjects
