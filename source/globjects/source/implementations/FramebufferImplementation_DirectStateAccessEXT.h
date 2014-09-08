#pragma once

#include <globjects/base/Singleton.h>

#include "AbstractFramebufferImplementation.h"


namespace globjects
{

class FramebufferImplementation_DirectStateAccessEXT : public AbstractFramebufferImplementation
    , public Singleton<FramebufferImplementation_DirectStateAccessEXT>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;

    virtual gl::GLenum checkStatus(const Framebuffer * fbo) const override;
    virtual void setParameter(const Framebuffer * fbo, gl::GLenum pname, gl::GLint param) const override;
    virtual gl::GLint getAttachmentParameter(const Framebuffer * fbo, gl::GLenum attachment, gl::GLenum pname) const override;
    virtual void attachTexture(const Framebuffer * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const override;
    virtual void attachTextureLayer(const Framebuffer * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const override;
    virtual void attachRenderBuffer(const Framebuffer * fbo, gl::GLenum attachment, Renderbuffer * renderBuffer) const override;
    virtual void setReadBuffer(const Framebuffer * fbo, gl::GLenum mode) const override;
    virtual void setDrawBuffer(const Framebuffer * fbo, gl::GLenum mode) const override;
    virtual void setDrawBuffers(const Framebuffer * fbo, gl::GLsizei n, const gl::GLenum * modes) const override;
};

} // namespace globjects
