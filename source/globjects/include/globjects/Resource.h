
#pragma once

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>


namespace globjects 
{


class GLOBJECTS_API AbstractResource
{
public:
    AbstractResource(bool hasOwnership);
    virtual ~AbstractResource();

    bool hasOwnership() const;

protected:
    bool m_hasOwnership;
};


class GLOBJECTS_API IDTrait
{
public:
    IDTrait(gl::GLuint id);
    virtual ~IDTrait();

    gl::GLuint id() const;

protected:
    gl::GLuint m_id;
};


class GLOBJECTS_API IDResource : public AbstractResource, public IDTrait
{
public:
    IDResource(gl::GLuint id);

protected:
    IDResource(gl::GLuint id, bool hasOwnership);
};


class GLOBJECTS_API ExternalResource : public IDResource
{
public:
    ExternalResource(gl::GLuint id);
};


class GLOBJECTS_API InvalidResource : public IDResource
{
public:
    InvalidResource();
    ~InvalidResource();
};


class GLOBJECTS_API BufferResource : public IDResource
{
public:
    BufferResource();
    ~BufferResource();
};


class GLOBJECTS_API FrameBufferObjectResource : public IDResource
{
public:
    FrameBufferObjectResource();
    ~FrameBufferObjectResource();
};


class GLOBJECTS_API ProgramResource : public IDResource
{
public:
    ProgramResource();
    ~ProgramResource();
};


class GLOBJECTS_API ProgramPipelineResource : public IDResource
{
public:
    ProgramPipelineResource();
    ~ProgramPipelineResource();
};


class GLOBJECTS_API QueryResource : public IDResource
{
public:
    QueryResource();
    ~QueryResource();
};


class GLOBJECTS_API RenderBufferObjectResource : public IDResource
{
public:
    RenderBufferObjectResource();
    ~RenderBufferObjectResource();
};


class GLOBJECTS_API SamplerResource : public IDResource
{
public:
    SamplerResource();
    ~SamplerResource();
};


class GLOBJECTS_API ShaderResource : public IDResource
{
public:
    ShaderResource(gl::GLenum type);
    ~ShaderResource();
};


class GLOBJECTS_API TextureResource : public IDResource
{
public:
    TextureResource(gl::GLenum target);
    ~TextureResource();
};


class GLOBJECTS_API TransformFeedbackResource : public IDResource
{
public:
    TransformFeedbackResource();
    ~TransformFeedbackResource();
};


class GLOBJECTS_API VertexArrayObjectResource : public IDResource
{
public:
    VertexArrayObjectResource();
    ~VertexArrayObjectResource();
};


} // namespace globjects
