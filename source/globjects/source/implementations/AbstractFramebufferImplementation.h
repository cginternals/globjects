
#pragma once

#include <glbinding/gl/types.h>

#include <globjects/Framebuffer.h>


namespace globjects
{


class Framebuffer;
class Texture;
class Renderbuffer;

class AbstractFramebufferImplementation
{
public:
    AbstractFramebufferImplementation();
    virtual ~AbstractFramebufferImplementation();

    static AbstractFramebufferImplementation * get(Framebuffer::BindlessImplementation impl = 
        Framebuffer::BindlessImplementation::DirectStateAccessARB);


    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;

    virtual gl::GLenum checkStatus(const Framebuffer * fbo) const = 0;
    virtual void setParameter(const Framebuffer * fbo, gl::GLenum pname, gl::GLint param) const = 0;
    virtual gl::GLint getAttachmentParameter(const Framebuffer * fbo, gl::GLenum attachment, gl::GLenum pname) const = 0;
    virtual void attachTexture(const Framebuffer * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const = 0;
    virtual void attachTextureLayer(const Framebuffer * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const = 0;
    virtual void attachRenderBuffer(const Framebuffer * fbo, gl::GLenum attachment, Renderbuffer * renderBuffer) const = 0;
    virtual void setReadBuffer(const Framebuffer * fbo, gl::GLenum mode) const = 0;
    virtual void setDrawBuffer(const Framebuffer * fbo, gl::GLenum mode) const = 0;
    virtual void setDrawBuffers(const Framebuffer * fbo, gl::GLsizei n, const gl::GLenum * modes) const = 0;

    virtual void clear(const Framebuffer * fbo, gl::ClearBufferMask mask) const = 0;
    virtual void clearBufferiv(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLint * value) const = 0;
    virtual void clearBufferuiv(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLuint * value) const = 0;
    virtual void clearBufferfv(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, const gl::GLfloat * value) const = 0;
    virtual void clearBufferfi(const Framebuffer * fbo, gl::GLenum buffer, gl::GLint drawBuffer, gl::GLfloat depth, gl::GLint stencil) const = 0;

    virtual void readPixels(const Framebuffer * fbo, const gl::GLint x, const gl::GLint y, const gl::GLsizei width, const gl::GLsizei height, const gl::GLenum format, const gl::GLenum type, gl::GLvoid * data) const = 0;

    virtual void blit(const Framebuffer * sourceFbo, const Framebuffer * targetFbo, gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::ClearBufferMask mask, gl::GLenum filter) const = 0;

public:
    static gl::GLenum s_workingTarget;
};


} // namespace globjects
