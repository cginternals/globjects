
#pragma once

#include "../base/Singleton.h"

#include "AbstractFramebufferImplementation.h"


namespace globjects
{


class FramebufferImplementation_Legacy : public AbstractFramebufferImplementation
    , public Singleton<FramebufferImplementation_Legacy>
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

    virtual void clear(const Framebuffer * fbo, gl::ClearBufferMask mask) const override;
    virtual void clearBufferiv(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLint * value) const override;
    virtual void clearBufferuiv(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLuint * value) const override;
    virtual void clearBufferfv(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLfloat * value) const override;
    virtual void clearBufferfi(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, gl::GLfloat depth, gl::GLint stencil) const override;

    virtual void readPixels(const Framebuffer * fbo, const gl::GLint x, const gl::GLint y, const gl::GLsizei width, const gl::GLsizei height, const gl::GLenum format, const gl::GLenum type, gl::GLvoid * data) const override;

    virtual void blit(const Framebuffer * sourceFbo, const Framebuffer * targetFbo, gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::ClearBufferMask mask, gl::GLenum filter) const override;
};


} // namespace globjects
