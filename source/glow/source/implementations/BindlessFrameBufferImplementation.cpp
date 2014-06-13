#include "BindlessFrameBufferImplementation.h"

#include <glbinding/functions.h>

#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>

namespace glow {

gl::GLenum BindlessFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    return gl::CheckNamedFramebufferStatusEXT(fbo->id(), gl::FRAMEBUFFER);
}

void BindlessFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const
{
    gl::NamedFramebufferParameteriEXT(fbo->id(), pname, param);
}

gl::GLint BindlessFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const
{
    gl::GLint result = 0;

    gl::GetNamedFramebufferAttachmentParameterivEXT(fbo->id(), attachment, pname, &result);

    return result;
}

void BindlessFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    gl::NamedFramebufferTextureEXT(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void BindlessFrameBufferImplementation::attachTexture1D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    gl::NamedFramebufferTexture1DEXT(fbo->id(), attachment, texture ? texture->target() : gl::TEXTURE_1D, texture ? texture->id() : 0, level);
}

void BindlessFrameBufferImplementation::attachTexture2D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    gl::NamedFramebufferTexture2DEXT(fbo->id(), attachment, texture ? texture->target() : gl::TEXTURE_2D, texture ? texture->id() : 0, level);
}

void BindlessFrameBufferImplementation::attachTexture3D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    gl::NamedFramebufferTexture3DEXT(fbo->id(), attachment, texture ? texture->target() : gl::TEXTURE_3D, texture ? texture->id() : 0, level, layer);
}

void BindlessFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    gl::NamedFramebufferTextureLayerEXT(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void BindlessFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(gl::RENDERBUFFER); // TODO: is this necessary?

    gl::NamedFramebufferRenderbufferEXT(fbo->id(), attachment, gl::RENDERBUFFER, renderBuffer->id());
}

void BindlessFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    gl::FramebufferReadBufferEXT(fbo->id(), mode);
}

void BindlessFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    gl::FramebufferDrawBufferEXT(fbo->id(), mode);
}

void BindlessFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const
{
    gl::FramebufferDrawBuffersEXT(fbo->id(), n, modes);
}

} // namespace glow
