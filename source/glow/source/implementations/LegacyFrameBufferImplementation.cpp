#include "LegacyFrameBufferImplementation.h"

#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Error.h>

namespace glow {

GLenum LegacyFrameBufferImplementation::s_workingTarget = GL_FRAMEBUFFER;

GLenum LegacyFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    fbo->bind(s_workingTarget);

    GLenum result = glCheckFramebufferStatus(s_workingTarget);
    CheckGLError();

    return result;
}

void LegacyFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const
{
    fbo->bind(s_workingTarget);

    glFramebufferParameteri(s_workingTarget, pname, param);
    CheckGLError();
}

GLint LegacyFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const
{
    fbo->bind(s_workingTarget);

    GLint result = 0;

    glGetFramebufferAttachmentParameteriv(s_workingTarget, attachment, pname, &result);
    CheckGLError();

    return result;
}

void LegacyFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    fbo->bind(s_workingTarget);

    glFramebufferTexture(s_workingTarget, attachment, texture ? texture->id() : 0, level);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTexture1D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    fbo->bind(s_workingTarget);

    glFramebufferTexture1D(s_workingTarget, attachment, texture ? texture->target() : GL_TEXTURE_1D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTexture2D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    fbo->bind(s_workingTarget);

    glFramebufferTexture2D(s_workingTarget, attachment, texture ? texture->target() : GL_TEXTURE_2D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTexture3D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    fbo->bind(s_workingTarget);

    glFramebufferTexture3D(s_workingTarget, attachment, texture ? texture->target() : GL_TEXTURE_3D, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    fbo->bind(s_workingTarget);

    glFramebufferTextureLayer(s_workingTarget, attachment, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void LegacyFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const
{
    fbo->bind(s_workingTarget);
    renderBuffer->bind(GL_RENDERBUFFER);

    glFramebufferRenderbuffer(s_workingTarget, attachment, GL_RENDERBUFFER, renderBuffer->id());
    CheckGLError();
}

void LegacyFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    fbo->bind(GL_READ_FRAMEBUFFER);

    glReadBuffer(mode);
    CheckGLError();
}

void LegacyFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glDrawBuffer(mode);
    CheckGLError();
}

void LegacyFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glDrawBuffers(n, modes);
    CheckGLError();
}

} // namespace glow
