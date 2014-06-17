#include "LegacyFrameBufferImplementation.h"

#include <glbinding/functions.h>

#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>

namespace glow {

gl::GLenum LegacyFrameBufferImplementation::s_workingTarget = gl::GL_FRAMEBUFFER;

gl::GLenum LegacyFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    fbo->bind(s_workingTarget);

    return gl::glCheckFramebufferStatus(s_workingTarget);
}

void LegacyFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const
{
    fbo->bind(s_workingTarget);

    gl::glFramebufferParameteri(s_workingTarget, pname, param);
}

gl::GLint LegacyFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const
{
    fbo->bind(s_workingTarget);

    gl::GLint result = 0;

    gl::glGetFramebufferAttachmentParameteriv(s_workingTarget, attachment, pname, &result);

    return result;
}

void LegacyFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::glFramebufferTexture(s_workingTarget, attachment, texture ? texture->id() : 0, level);
}

void LegacyFrameBufferImplementation::attachTexture1D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::glFramebufferTexture1D(s_workingTarget, attachment, texture ? texture->target() : gl::GL_TEXTURE_1D, texture ? texture->id() : 0, level);
}

void LegacyFrameBufferImplementation::attachTexture2D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::glFramebufferTexture2D(s_workingTarget, attachment, texture ? texture->target() : gl::GL_TEXTURE_2D, texture ? texture->id() : 0, level);
}

void LegacyFrameBufferImplementation::attachTexture3D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    fbo->bind(s_workingTarget);

    gl::glFramebufferTexture3D(s_workingTarget, attachment, texture ? texture->target() : gl::GL_TEXTURE_3D, texture ? texture->id() : 0, level, layer);
}

void LegacyFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    fbo->bind(s_workingTarget);

    gl::glFramebufferTextureLayer(s_workingTarget, attachment, texture ? texture->id() : 0, level, layer);
}

void LegacyFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const
{
    fbo->bind(s_workingTarget);
    renderBuffer->bind(gl::GL_RENDERBUFFER);

    gl::glFramebufferRenderbuffer(s_workingTarget, attachment, gl::GL_RENDERBUFFER, renderBuffer->id());
}

void LegacyFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    fbo->bind(gl::GL_READ_FRAMEBUFFER);

    gl::glReadBuffer(mode);
}

void LegacyFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    fbo->bind(gl::GL_DRAW_FRAMEBUFFER);

    gl::glDrawBuffer(mode);
}

void LegacyFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const
{
    fbo->bind(gl::GL_DRAW_FRAMEBUFFER);

    gl::glDrawBuffers(n, modes);
}

} // namespace glow
