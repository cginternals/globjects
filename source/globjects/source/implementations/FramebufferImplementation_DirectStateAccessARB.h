#pragma once

#include <globjects-base/Singleton.h>

#include "AbstractFramebufferImplementation.h"


namespace glo
{

class FramebufferImplementation_DirectStateAccessARB : public AbstractFramebufferImplementation
    , public Singleton<FramebufferImplementation_DirectStateAccessARB>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;

    virtual gl::GLenum checkStatus(const FrameBufferObject * fbo) const override;
    virtual void setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const override;
    virtual gl::GLint getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const override;
    virtual void attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const override;
    virtual void attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const override;
    virtual void attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const override;
    virtual void setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const override;
    virtual void setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const override;
    virtual void setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const override;
};

} // namespace glo
