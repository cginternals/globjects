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

public:
    static gl::GLenum s_workingTarget;
};

} // namespace globjects
