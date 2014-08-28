
#include "BindlessEXTFrameBufferImplementation.h"

#include <glbinding/gl/functions.h>

#include <globjects/FrameBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>

using namespace gl;

namespace glo 
{

GLuint BindlessEXTFrameBufferImplementation::create() const
{
    // ToDo: use legacy impl. (singleton impls)

    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer); // create a handle to a potentially used framebuffer
    glBindFramebuffer(s_workingTarget, framebuffer); // trigger actual framebuffer creation

    return framebuffer;
}

void BindlessEXTFrameBufferImplementation::destroy(GLuint id) const
{
    glDeleteFramebuffers(1, &id);
}

GLenum BindlessEXTFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    return glCheckNamedFramebufferStatusEXT(fbo->id(), GL_FRAMEBUFFER);
}

void BindlessEXTFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const
{
    glNamedFramebufferParameteriEXT(fbo->id(), pname, param);
}

GLint BindlessEXTFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const
{
    GLint result = 0;

    glGetNamedFramebufferAttachmentParameterivEXT(fbo->id(), attachment, pname, &result);

    return result;
}

void BindlessEXTFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTextureEXT(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void BindlessEXTFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTextureLayerEXT(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void BindlessEXTFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(GL_RENDERBUFFER); // TODO: is this necessary?

    glNamedFramebufferRenderbufferEXT(fbo->id(), attachment, GL_RENDERBUFFER, renderBuffer->id());
}

void BindlessEXTFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glFramebufferReadBufferEXT(fbo->id(), mode);
}

void BindlessEXTFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glFramebufferDrawBufferEXT(fbo->id(), mode);
}

void BindlessEXTFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const
{
    glFramebufferDrawBuffersEXT(fbo->id(), n, modes);
}

} // namespace glo
