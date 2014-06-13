#pragma once

#include <glbinding/types.h>

namespace glow
{

class FrameBufferObject;
class Texture;
class RenderBufferObject;

class AbstractFrameBufferImplementation
{
public:
    AbstractFrameBufferImplementation();
    virtual ~AbstractFrameBufferImplementation();

    static AbstractFrameBufferImplementation * create();

    virtual gl::GLenum checkStatus(const FrameBufferObject * fbo) const = 0;
    virtual void setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const = 0;
    virtual gl::GLint getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const = 0;
    virtual void attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const = 0;
    virtual void attachTexture1D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const = 0;
    virtual void attachTexture2D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const = 0;
    virtual void attachTexture3D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const = 0;
    virtual void attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const = 0;
    virtual void attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const = 0;
    virtual void setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const = 0;
    virtual void setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const = 0;
    virtual void setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const = 0;
};

} // namespace glow
