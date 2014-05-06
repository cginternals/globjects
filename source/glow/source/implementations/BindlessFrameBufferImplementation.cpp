#include "BindlessFrameBufferImplementation.h"

#include <glow/Error.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>

namespace glow {

GLenum BindlessFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    GLenum result = glCheckNamedFramebufferStatusEXT(fbo->id(), GL_FRAMEBUFFER);
    CheckGLError();

    return result;
}

void BindlessFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const
{
    glNamedFramebufferParameteriEXT(fbo->id(), pname, param);
    CheckGLError();
}

GLint BindlessFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const
{
    GLint result = 0;

    glGetNamedFramebufferAttachmentParameterivEXT(fbo->id(), attachment, pname, &result);
    CheckGLError();

    return result;
}

void BindlessFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTextureEXT(fbo->id(), attachment, texture ? texture->id() : 0, level);
    CheckGLError();
}

void BindlessFrameBufferImplementation::attachTexture1D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTexture1DEXT(fbo->id(), attachment, texture ? texture->target() : GL_TEXTURE_1D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void BindlessFrameBufferImplementation::attachTexture2D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTexture2DEXT(fbo->id(), attachment, texture ? texture->target() : GL_TEXTURE_2D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void BindlessFrameBufferImplementation::attachTexture3D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTexture3DEXT(fbo->id(), attachment, texture ? texture->target() : GL_TEXTURE_3D, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void BindlessFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTextureLayerEXT(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void BindlessFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(GL_RENDERBUFFER); // TODO: is this necessary?

    glNamedFramebufferRenderbufferEXT(fbo->id(), attachment, GL_RENDERBUFFER, renderBuffer->id());
    CheckGLError();
}

void BindlessFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glFramebufferReadBufferEXT(fbo->id(), mode);
    CheckGLError();
}

void BindlessFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glFramebufferDrawBufferEXT(fbo->id(), mode);
    CheckGLError();
}

void BindlessFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const
{
    glFramebufferDrawBuffersEXT(fbo->id(), n, modes);
    CheckGLError();
}

} // namespace glow
