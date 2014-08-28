#include "BindlessFrameBufferImplementation.h"

#include <glbinding/gl/functions.h>

#include <globjects/FrameBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>

namespace glo {

gl::GLenum BindlessFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    return gl::glCheckNamedFramebufferStatus(fbo->id(), gl::GL_FRAMEBUFFER);
}

void BindlessFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const
{
    gl::glNamedFramebufferParameteri(fbo->id(), pname, param);
}

gl::GLint BindlessFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const
{
    gl::GLint result = 0;

    gl::glGetNamedFramebufferAttachmentParameteriv(fbo->id(), attachment, pname, &result);

    return result;
}

void BindlessFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    gl::glNamedFramebufferTexture(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void BindlessFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    gl::glNamedFramebufferTextureLayer(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void BindlessFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(gl::GL_RENDERBUFFER); // TODO: is this necessary?

    gl::glNamedFramebufferRenderbuffer(fbo->id(), attachment, gl::GL_RENDERBUFFER, renderBuffer->id());
}

void BindlessFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    gl::glNamedFramebufferReadBuffer(fbo->id(), mode);
}

void BindlessFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    gl::glNamedFramebufferDrawBuffer(fbo->id(), mode);
}

void BindlessFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const
{
    gl::glNamedFramebufferDrawBuffers(fbo->id(), n, modes);
}

} // namespace glo
