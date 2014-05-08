#include "LegacyFrameBufferImplementation.h"

#include <glbinding/functions.h>

#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Error.h>

namespace glow {

gl::GLenum LegacyFrameBufferImplementation::s_workingTarget = gl::FRAMEBUFFER;

gl::GLenum LegacyFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    fbo->bind(s_workingTarget);

    return gl::CheckFramebufferStatus(s_workingTarget);
}

void LegacyFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferParameteri(s_workingTarget, pname, param);
}

gl::GLint LegacyFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const
{
    fbo->bind(s_workingTarget);

    gl::GLint result = 0;

    gl::GetFramebufferAttachmentParameteriv(s_workingTarget, attachment, pname, &result);

    return result;
}

void LegacyFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture(s_workingTarget, attachment, texture ? texture->id() : 0, level);
}

void LegacyFrameBufferImplementation::attachTexture1D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture1D(s_workingTarget, attachment, texture ? texture->target() : gl::TEXTURE_1D, texture ? texture->id() : 0, level);
}

void LegacyFrameBufferImplementation::attachTexture2D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture2D(s_workingTarget, attachment, texture ? texture->target() : gl::TEXTURE_2D, texture ? texture->id() : 0, level);
}

void LegacyFrameBufferImplementation::attachTexture3D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture3D(s_workingTarget, attachment, texture ? texture->target() : gl::TEXTURE_3D, texture ? texture->id() : 0, level, layer);
}

void LegacyFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTextureLayer(s_workingTarget, attachment, texture ? texture->id() : 0, level, layer);
}

void LegacyFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const
{
    fbo->bind(s_workingTarget);
    renderBuffer->bind(gl::RENDERBUFFER);

    gl::FramebufferRenderbuffer(s_workingTarget, attachment, gl::RENDERBUFFER, renderBuffer->id());
}

void LegacyFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    fbo->bind(gl::READ_FRAMEBUFFER);

    gl::ReadBuffer(mode);
}

void LegacyFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    fbo->bind(gl::DRAW_FRAMEBUFFER);

    gl::DrawBuffer(mode);
}

void LegacyFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const
{
    fbo->bind(gl::DRAW_FRAMEBUFFER);

    gl::DrawBuffers(n, modes);
}

} // namespace glow
