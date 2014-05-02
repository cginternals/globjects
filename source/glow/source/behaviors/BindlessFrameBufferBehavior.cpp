#include "BindlessFrameBufferBehavior.h"

#include <glow/Error.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>

namespace glow {

GLenum BindlessFrameBufferBehavior::checkStatus(const FrameBufferObject * fbo) const
{
    GLenum result = glCheckNamedFramebufferStatusEXT(fbo->id(), GL_FRAMEBUFFER);
    CheckGLError();

    return result;
}

void BindlessFrameBufferBehavior::setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const
{
    glNamedFramebufferParameteriEXT(fbo->id(), pname, param);
    CheckGLError();
}

GLint BindlessFrameBufferBehavior::getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const
{
    GLint result = 0;

    glGetNamedFramebufferAttachmentParameterivEXT(fbo->id(), attachment, pname, &result);
    CheckGLError();

    return result;
}

void BindlessFrameBufferBehavior::attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTextureEXT(fbo->id(), attachment, texture ? texture->id() : 0, level);
    CheckGLError();
}

void BindlessFrameBufferBehavior::attachTexture1D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTexture1DEXT(fbo->id(), attachment, texture ? texture->target() : GL_TEXTURE_1D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void BindlessFrameBufferBehavior::attachTexture2D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTexture2DEXT(fbo->id(), attachment, texture ? texture->target() : GL_TEXTURE_2D, texture ? texture->id() : 0, level);
    CheckGLError();
}

void BindlessFrameBufferBehavior::attachTexture3D(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTexture3DEXT(fbo->id(), attachment, texture ? texture->target() : GL_TEXTURE_3D, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void BindlessFrameBufferBehavior::attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTextureLayerEXT(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
    CheckGLError();
}

void BindlessFrameBufferBehavior::attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(GL_RENDERBUFFER); // TODO: is this necessary?

    glNamedFramebufferRenderbufferEXT(fbo->id(), attachment, GL_RENDERBUFFER, renderBuffer->id());
    CheckGLError();
}

} // namespace glow
