#pragma once

#include <GL/glew.h>

#include <glow/FrameBufferObject.h>

namespace glow
{

class AbstractFrameBufferImplementation
{
public:
    AbstractFrameBufferImplementation();
    virtual ~AbstractFrameBufferImplementation();

    static AbstractFrameBufferImplementation * create();

    virtual GLenum checkStatus(const FrameBufferObject * fbo) const = 0;
    virtual void setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const = 0;
    virtual GLint getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const = 0;
    virtual void attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const = 0;
    virtual void attachTexture1D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const = 0;
    virtual void attachTexture2D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const = 0;
    virtual void attachTexture3D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const = 0;
    virtual void attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const = 0;
    virtual void attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const = 0;
    virtual void setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const = 0;
    virtual void setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const = 0;
    virtual void setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const = 0;
};

} // namespace glow
