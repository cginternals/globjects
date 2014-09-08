
#include "FramebufferImplementation_DirectStateAccessEXT.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Renderbuffer.h>

#include "FramebufferImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

GLuint FramebufferImplementation_DirectStateAccessEXT::create() const
{
    return FramebufferImplementation_Legacy::instance()->create();
}

void FramebufferImplementation_DirectStateAccessEXT::destroy(GLuint id) const
{
    FramebufferImplementation_Legacy::instance()->destroy(id);
}

GLenum FramebufferImplementation_DirectStateAccessEXT::checkStatus(const Framebuffer * fbo) const
{
    return glCheckNamedFramebufferStatusEXT(fbo->id(), GL_FRAMEBUFFER);
}

void FramebufferImplementation_DirectStateAccessEXT::setParameter(const Framebuffer * fbo, GLenum pname, GLint param) const
{
    glNamedFramebufferParameteriEXT(fbo->id(), pname, param);
}

GLint FramebufferImplementation_DirectStateAccessEXT::getAttachmentParameter(const Framebuffer * fbo, GLenum attachment, GLenum pname) const
{
    GLint result = 0;

    glGetNamedFramebufferAttachmentParameterivEXT(fbo->id(), attachment, pname, &result);

    return result;
}

void FramebufferImplementation_DirectStateAccessEXT::attachTexture(const Framebuffer * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTextureEXT(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void FramebufferImplementation_DirectStateAccessEXT::attachTextureLayer(const Framebuffer * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTextureLayerEXT(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void FramebufferImplementation_DirectStateAccessEXT::attachRenderBuffer(const Framebuffer * fbo, GLenum attachment, Renderbuffer * renderBuffer) const
{
    renderBuffer->bind(); // TODO: is this necessary?

    glNamedFramebufferRenderbufferEXT(fbo->id(), attachment, GL_RENDERBUFFER, renderBuffer->id());
}

void FramebufferImplementation_DirectStateAccessEXT::setReadBuffer(const Framebuffer * fbo, GLenum mode) const
{
    glFramebufferReadBufferEXT(fbo->id(), mode);
}

void FramebufferImplementation_DirectStateAccessEXT::setDrawBuffer(const Framebuffer * fbo, GLenum mode) const
{
    glFramebufferDrawBufferEXT(fbo->id(), mode);
}

void FramebufferImplementation_DirectStateAccessEXT::setDrawBuffers(const Framebuffer * fbo, GLsizei n, const GLenum * modes) const
{
    glFramebufferDrawBuffersEXT(fbo->id(), n, modes);
}

} // namespace globjects
