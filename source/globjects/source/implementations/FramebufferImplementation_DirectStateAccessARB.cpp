
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
    // glCheckNamedFramebufferStatus seems to fail when checking the default framebuffer
    if (fbo->isDefault())
    {
        fbo->bind(GL_DRAW_FRAMEBUFFER);

        return glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    }

    return glCheckNamedFramebufferStatus(fbo->id(), GL_DRAW_FRAMEBUFFER);
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

void FramebufferImplementation_DirectStateAccessARB::clear(const Framebuffer *fbo, ClearBufferMask mask) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClear(mask);
}

void FramebufferImplementation_DirectStateAccessARB::clearBufferiv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLint * value) const
{
    glClearNamedFramebufferiv(fbo->id(), buffer, drawBuffer, value);
}

void FramebufferImplementation_DirectStateAccessARB::clearBufferuiv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLuint * value) const
{
    glClearNamedFramebufferuiv(fbo->id(),buffer, drawBuffer, value);
}

void FramebufferImplementation_DirectStateAccessARB::clearBufferfv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLfloat * value) const
{
    glClearNamedFramebufferfv(fbo->id(),buffer, drawBuffer, value);
}

void FramebufferImplementation_DirectStateAccessARB::clearBufferfi(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLfloat depth, const GLint stencil) const
{
    glClearNamedFramebufferfi(fbo->id(),buffer, drawBuffer, depth, stencil);
}

void FramebufferImplementation_DirectStateAccessARB::readPixels(const Framebuffer *fbo, const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, GLvoid * data) const
{
    fbo->bind(GL_READ_FRAMEBUFFER);

    glReadPixels(x, y, width, height, format, type, data);
}

void FramebufferImplementation_DirectStateAccessARB::blit(const Framebuffer * sourceFbo, const Framebuffer * targetFbo, gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::ClearBufferMask mask, gl::GLenum filter) const
{
    glBlitNamedFramebuffer(sourceFbo->id(), targetFbo->id(), srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);
}


} // namespace globjects
