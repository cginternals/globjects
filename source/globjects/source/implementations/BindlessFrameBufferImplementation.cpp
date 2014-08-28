
#include "BindlessFrameBufferImplementation.h"

#include <glbinding/gl/functions.h>

#include <globjects/FrameBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>


using namespace gl;

namespace glo 
{

GLuint BindlessFrameBufferImplementation::create() const
{
    GLuint frameBuffer;
    glCreateFramebuffers(1, &frameBuffer); // create a handle as well as the actual buffer

    return frameBuffer;
}

void BindlessFrameBufferImplementation::destroy(const GLuint id) const
{
    glDeleteFramebuffers(1, &id);
}

GLenum BindlessFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    return glCheckNamedFramebufferStatus(fbo->id(), GL_FRAMEBUFFER);
}

void BindlessFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const
{
    glNamedFramebufferParameteri(fbo->id(), pname, param);
}

GLint BindlessFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const
{
    GLint result = 0;

    glGetNamedFramebufferAttachmentParameteriv(fbo->id(), attachment, pname, &result);

    return result;
}

void BindlessFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTexture(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void BindlessFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTextureLayer(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void BindlessFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(GL_RENDERBUFFER); // TODO: is this necessary?

    glNamedFramebufferRenderbuffer(fbo->id(), attachment, GL_RENDERBUFFER, renderBuffer->id());
}

void BindlessFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glNamedFramebufferReadBuffer(fbo->id(), mode);
}

void BindlessFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glNamedFramebufferDrawBuffer(fbo->id(), mode);
}

void BindlessFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const
{
    glNamedFramebufferDrawBuffers(fbo->id(), n, modes);
}

} // namespace glo
