
#include "FramebufferImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Renderbuffer.h>


using namespace gl;


namespace globjects 
{


GLuint FramebufferImplementation_Legacy::create() const
{
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer); // create a handle to a potentially used framebuffer
    glBindFramebuffer(s_workingTarget, framebuffer); // trigger actual framebuffer creation

    return framebuffer;
}

void FramebufferImplementation_Legacy::destroy(GLuint id) const
{
    glDeleteFramebuffers(1, &id);
}

GLenum FramebufferImplementation_Legacy::checkStatus(const Framebuffer * fbo) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    return glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
}

void FramebufferImplementation_Legacy::setParameter(const Framebuffer * fbo, GLenum pname, GLint param) const
{
    fbo->bind(s_workingTarget);

    glFramebufferParameteri(s_workingTarget, pname, param);
}

GLint FramebufferImplementation_Legacy::getAttachmentParameter(const Framebuffer * fbo, GLenum attachment, GLenum pname) const
{
    fbo->bind(s_workingTarget);

    GLint result = 0;

    glGetFramebufferAttachmentParameteriv(s_workingTarget, attachment, pname, &result);

    return result;
}

void FramebufferImplementation_Legacy::attachTexture(const Framebuffer * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    fbo->bind(s_workingTarget);

    if (texture == nullptr)
    {
        glFramebufferTexture(s_workingTarget, attachment, 0, level);
    }
    else
    {
        switch (texture->target())
        {
        case GL_TEXTURE_1D:
            glFramebufferTexture1D(s_workingTarget, attachment, texture->target(), texture->id(), level);
            break;
        case GL_TEXTURE_2D:
        case GL_TEXTURE_RECTANGLE:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        case GL_TEXTURE_2D_MULTISAMPLE:
            glFramebufferTexture2D(s_workingTarget, attachment, texture->target(), texture->id(), level);
            break;
        default:
            glFramebufferTexture(s_workingTarget, attachment, texture->id(), level);
            break;
        }
    }
}

void FramebufferImplementation_Legacy::attachTextureLayer(const Framebuffer * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    fbo->bind(s_workingTarget);

    glFramebufferTextureLayer(s_workingTarget, attachment, texture ? texture->id() : 0, level, layer);
}

void FramebufferImplementation_Legacy::attachRenderBuffer(const Framebuffer * fbo, GLenum attachment, Renderbuffer * renderBuffer) const
{
    fbo->bind(s_workingTarget);
    renderBuffer->bind();

    glFramebufferRenderbuffer(s_workingTarget, attachment, GL_RENDERBUFFER, renderBuffer->id());
}

void FramebufferImplementation_Legacy::setReadBuffer(const Framebuffer * fbo, GLenum mode) const
{
    fbo->bind(GL_READ_FRAMEBUFFER);

    glReadBuffer(mode);
}

void FramebufferImplementation_Legacy::setDrawBuffer(const Framebuffer * fbo, GLenum mode) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glDrawBuffer(mode);
}

void FramebufferImplementation_Legacy::setDrawBuffers(const Framebuffer * fbo, GLsizei n, const GLenum * modes) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glDrawBuffers(n, modes);
}

void FramebufferImplementation_Legacy::clear(const Framebuffer *fbo, ClearBufferMask mask) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClear(mask);
}

void FramebufferImplementation_Legacy::clearBufferiv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLint * value) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferiv(buffer, drawBuffer, value);
}

void FramebufferImplementation_Legacy::clearBufferuiv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLuint * value) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferuiv(buffer, drawBuffer, value);
}

void FramebufferImplementation_Legacy::clearBufferfv(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLfloat * value) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferfv(buffer, drawBuffer, value);
}

void FramebufferImplementation_Legacy::clearBufferfi(const Framebuffer *fbo, const GLenum buffer, const GLint drawBuffer, const GLfloat depth, const GLint stencil) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glClearBufferfi(buffer, drawBuffer, depth, stencil);
}

void FramebufferImplementation_Legacy::readPixels(const Framebuffer *fbo, const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, GLvoid * data) const
{
    fbo->bind(GL_READ_FRAMEBUFFER);

    glReadPixels(x, y, width, height, format, type, data);
}

void FramebufferImplementation_Legacy::blit(const Framebuffer * sourceFbo, const Framebuffer * targetFbo, gl::GLint srcX0, gl::GLint srcY0, gl::GLint srcX1, gl::GLint srcY1, gl::GLint destX0, gl::GLint destY0, gl::GLint destX1, gl::GLint destY1, gl::ClearBufferMask mask, gl::GLenum filter) const
{
    sourceFbo->bind(GL_READ_FRAMEBUFFER);
    targetFbo->bind(GL_DRAW_FRAMEBUFFER);

    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, destX0, destY0, destX1, destY1, mask, filter);
}


} // namespace globjects
