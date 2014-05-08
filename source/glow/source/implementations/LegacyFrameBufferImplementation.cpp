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

    gl::GLenum result = gl::CheckFramebufferStatus(s_workingTarget);
    CheckGLError();

    return result;
}

void LegacyFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferParameteri(s_workingTarget, pname, param);
    CheckGLError();
}

gl::GLint LegacyFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const
{
    fbo->bind(s_workingTarget);

    gl::GLint result = 0;

    gl::GetFramebufferAttachmentParameteriv(s_workingTarget, attachment, pname, &result);
    CheckGLError();

    return result;
}

void LegacyFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture(s_workingTarget, attachment, texture ? texture->id() : 0, level);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTexture1D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture1D(s_workingTarget, attachment, texture ? texture->target() : gl::TEXTURE_1D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTexture2D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture2D(s_workingTarget, attachment, texture ? texture->target() : gl::TEXTURE_2D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTexture3D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTexture3D(s_workingTarget, attachment, texture ? texture->target() : gl::TEXTURE_3D, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const
{
    fbo->bind(s_workingTarget);

    gl::FramebufferTextureLayer(s_workingTarget, attachment, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const
{
    fbo->bind(s_workingTarget);
    renderBuffer->bind(gl::RENDERBUFFER);

    gl::FramebufferRenderbuffer(s_workingTarget, attachment, gl::RENDERBUFFER, renderBuffer->id());
    CheckGLError();
}

void LegacyFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    fbo->bind(gl::READ_FRAMEBUFFER);

    gl::ReadBuffer(mode);
    CheckGLError();
}

void LegacyFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const
{
    fbo->bind(gl::DRAW_FRAMEBUFFER);

    gl::DrawBuffer(mode);
    CheckGLError();
}

void LegacyFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const
{
    fbo->bind(gl::DRAW_FRAMEBUFFER);

    gl::DrawBuffers(n, modes);
    CheckGLError();
}

} // namespace glow
