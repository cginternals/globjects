
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
    // glCheckNamedFramebufferStatus seems to fail when checking the default framebuffer
    if (fbo->isDefault())
    {
        fbo->bind(GL_DRAW_FRAMEBUFFER);

        return glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    }

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

void FramebufferImplementation_DirectStateAccessEXT::clear(const Framebuffer *fbo, ClearBufferMask mask) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClear(mask);
}

void FramebufferImplementation_DirectStateAccessEXT::clearBufferiv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLint * value) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferiv(buffer, drawBuffer, value);
}

void FramebufferImplementation_DirectStateAccessEXT::clearBufferuiv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLuint * value) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferuiv(buffer, drawBuffer, value);
}

void FramebufferImplementation_DirectStateAccessEXT::clearBufferfv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLfloat * value) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferfv(buffer, drawBuffer, value);
}

void FramebufferImplementation_DirectStateAccessEXT::clearBufferfi(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLfloat depth, const GLint stencil) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferfi(buffer, drawBuffer, depth, stencil);
}

void FramebufferImplementation_DirectStateAccessEXT::readPixels(const Framebuffer *fbo, const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, GLvoid * data) const
{
    fbo->bind(GL_READ_FRAMEBUFFER);

    glReadPixels(x, y, width, height, format, type, data);
}

void FramebufferImplementation_DirectStateAccessEXT::blit(const Framebuffer * sourceFbo, const Framebuffer * targetFbo, gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::ClearBufferMask mask, gl::GLenum filter) const
{
    sourceFbo->bind(GL_READ_FRAMEBUFFER);
    targetFbo->bind(GL_DRAW_FRAMEBUFFER);

    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);
}


} // namespace globjects
