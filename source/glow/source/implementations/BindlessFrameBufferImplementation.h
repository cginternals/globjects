#pragma once

#include <GL/glew.h>

#include "AbstractFrameBufferImplementation.h"

namespace glow
{

class BindlessFrameBufferImplementation : public AbstractFrameBufferImplementation
{
public:
    virtual GLenum checkStatus(const FrameBufferObject * fbo) const override;
    virtual void setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const override;
    virtual GLint getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const override;
    virtual void attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const override;
    virtual void attachTexture1D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const override;
    virtual void attachTexture2D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const override;
    virtual void attachTexture3D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const override;
    virtual void attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const override;
    virtual void attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const override;
    virtual void setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const override;
    virtual void setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const override;
    virtual void setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const override;
};

} // namespace glow
