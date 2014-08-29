
#include "FramebufferImplementation_DirectStateAccessEXT.h"

#include <glbinding/gl/functions.h>

#include <globjects/FrameBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>

#include "FramebufferImplementation_Legacy.h"


using namespace gl;

namespace glo 
{

GLuint FramebufferImplementation_DirectStateAccessEXT::create() const
{
    return FramebufferImplementation_Legacy::instance()->create();
}

void FramebufferImplementation_DirectStateAccessEXT::destroy(GLuint id) const
{
    FramebufferImplementation_Legacy::instance()->destroy(id);
}

GLenum FramebufferImplementation_DirectStateAccessEXT::checkStatus(const FrameBufferObject * fbo) const
{
    return glCheckNamedFramebufferStatusEXT(fbo->id(), GL_FRAMEBUFFER);
}

void FramebufferImplementation_DirectStateAccessEXT::setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const
{
    glNamedFramebufferParameteriEXT(fbo->id(), pname, param);
}

GLint FramebufferImplementation_DirectStateAccessEXT::getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const
{
    GLint result = 0;

    glGetNamedFramebufferAttachmentParameterivEXT(fbo->id(), attachment, pname, &result);

    return result;
}

void FramebufferImplementation_DirectStateAccessEXT::attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    glNamedFramebufferTextureEXT(fbo->id(), attachment, texture ? texture->id() : 0, level);
}

void FramebufferImplementation_DirectStateAccessEXT::attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    glNamedFramebufferTextureLayerEXT(fbo->id(), attachment, texture ? texture->id() : 0, level, layer);
}

void FramebufferImplementation_DirectStateAccessEXT::attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const
{
    renderBuffer->bind(GL_RENDERBUFFER); // TODO: is this necessary?

    glNamedFramebufferRenderbufferEXT(fbo->id(), attachment, GL_RENDERBUFFER, renderBuffer->id());
}

void FramebufferImplementation_DirectStateAccessEXT::setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glFramebufferReadBufferEXT(fbo->id(), mode);
}

void FramebufferImplementation_DirectStateAccessEXT::setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    glFramebufferDrawBufferEXT(fbo->id(), mode);
}

void FramebufferImplementation_DirectStateAccessEXT::setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const
{
    glFramebufferDrawBuffersEXT(fbo->id(), n, modes);
}

} // namespace glo
