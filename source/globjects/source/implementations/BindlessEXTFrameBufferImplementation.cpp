#include "BindlessEXTFrameBufferImplementation.h"

#include <glbinding/gl/functions.h>

#include <globjects/FrameBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>

namespace glo {

gl::GLenum BindlessEXTFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    return gl::glCheckNamedFramebufferStatusEXT(fbo->id(), gl::GL_FRAMEBUFFER);
}

void BindlessEXTFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const
{
    gl::glNamedFramebufferParameteriEXT(fbo->id(), pname, param);
}

gl::GLint BindlessEXTFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const
{
    gl::GLint result = 0;

    gl::glGetNamedFramebufferAttachmentParameterivEXT(fbo->id(), attachment, pname, &result);

    return result;
}

void BindlessEXTFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    gl::glNamedFramebufferTextureEXT(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void BindlessEXTFrameBufferImplementation::attachTexture1D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    gl::glNamedFramebufferTexture1DEXT(fbo->id(), attachment, texture ? texture->target() : gl::GL_TEXTURE_1D, texture ? texture->id() : 0, level);
}

void BindlessEXTFrameBufferImplementation::attachTexture2D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    gl::glNamedFramebufferTexture2DEXT(fbo->id(), attachment, texture ? texture->target() : gl::GL_TEXTURE_2D, texture ? texture->id() : 0, level);
}

void BindlessEXTFrameBufferImplementation::attachTexture3D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    gl::glNamedFramebufferTexture3DEXT(fbo->id(), attachment, texture ? texture->target() : gl::GL_TEXTURE_3D, texture ? texture->id() : 0, level, layer);
}

void BindlessEXTFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    gl::glNamedFramebufferTextureLayerEXT(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void BindlessEXTFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(gl::GL_RENDERBUFFER); // TODO: is this necessary?

    gl::glNamedFramebufferRenderbufferEXT(fbo->id(), attachment, gl::GL_RENDERBUFFER, renderBuffer->id());
}

void BindlessEXTFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    gl::glFramebufferReadBufferEXT(fbo->id(), mode);
}

void BindlessEXTFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    gl::glFramebufferDrawBufferEXT(fbo->id(), mode);
}

void BindlessEXTFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const
{
    gl::glFramebufferDrawBuffersEXT(fbo->id(), n, modes);
}

} // namespace glo
