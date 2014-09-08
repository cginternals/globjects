
#include "FramebufferImplementation_DirectStateAccessARB.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Renderbuffer.h>

#include "FramebufferImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

GLuint FramebufferImplementation_DirectStateAccessARB::create() const
{
    GLuint frameBuffer;
    glCreateFramebuffers(1, &frameBuffer); // create a handle as well as the actual buffer

    return frameBuffer;
}

void FramebufferImplementation_DirectStateAccessARB::destroy(const GLuint id) const
{
    FramebufferImplementation_Legacy::instance()->destroy(id);
}

GLenum FramebufferImplementation_DirectStateAccessARB::checkStatus(const Framebuffer * fbo) const
{
    return glCheckNamedFramebufferStatus(fbo->id(), GL_FRAMEBUFFER);
}

void FramebufferImplementation_DirectStateAccessARB::setParameter(const Framebuffer * fbo, GLenum pname, GLint param) const
{
    glNamedFramebufferParameteri(fbo->id(), pname, param);
}

GLint FramebufferImplementation_DirectStateAccessARB::getAttachmentParameter(const Framebuffer * fbo, GLenum attachment, GLenum pname) const
{
    GLint result = 0;

    glGetNamedFramebufferAttachmentParameteriv(fbo->id(), attachment, pname, &result);

    return result;
}

void FramebufferImplementation_DirectStateAccessARB::attachTexture(const Framebuffer * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTexture(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void FramebufferImplementation_DirectStateAccessARB::attachTextureLayer(const Framebuffer * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTextureLayer(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void FramebufferImplementation_DirectStateAccessARB::attachRenderBuffer(const Framebuffer * fbo, GLenum attachment, Renderbuffer * renderBuffer) const
{
    renderBuffer->bind(); // TODO: is this necessary?

    glNamedFramebufferRenderbuffer(fbo->id(), attachment, GL_RENDERBUFFER, renderBuffer->id());
}

void FramebufferImplementation_DirectStateAccessARB::setReadBuffer(const Framebuffer * fbo, GLenum mode) const
{
    glNamedFramebufferReadBuffer(fbo->id(), mode);
}

void FramebufferImplementation_DirectStateAccessARB::setDrawBuffer(const Framebuffer * fbo, GLenum mode) const
{
    glNamedFramebufferDrawBuffer(fbo->id(), mode);
}

void FramebufferImplementation_DirectStateAccessARB::setDrawBuffers(const Framebuffer * fbo, GLsizei n, const GLenum * modes) const
{
    glNamedFramebufferDrawBuffers(fbo->id(), n, modes);
}

} // namespace globjects
