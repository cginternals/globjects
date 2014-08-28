
#include "LegacyFrameBufferImplementation.h"

#include <glbinding/gl/functions.h>

#include <globjects/FrameBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>


using namespace gl;

namespace glo 
{

GLuint LegacyFrameBufferImplementation::create() const
{
    // ToDo: use legacy impl. (singleton impls)

    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer); // create a handle to a potentially used framebuffer
    glBindFramebuffer(s_workingTarget, framebuffer); // trigger actual framebuffer creation

    return framebuffer;
}

void LegacyFrameBufferImplementation::destroy(GLuint id) const
{
    glDeleteFramebuffers(1, &id);
}

GLenum LegacyFrameBufferImplementation::checkStatus(const FrameBufferObject * fbo) const
{
    fbo->bind(s_workingTarget);

    return glCheckFramebufferStatus(s_workingTarget);
}

void LegacyFrameBufferImplementation::setParameter(const FrameBufferObject * fbo, GLenum pname, GLint param) const
{
    fbo->bind(s_workingTarget);

    glFramebufferParameteri(s_workingTarget, pname, param);
}

GLint LegacyFrameBufferImplementation::getAttachmentParameter(const FrameBufferObject * fbo, GLenum attachment, GLenum pname) const
{
    fbo->bind(s_workingTarget);

    GLint result = 0;

    glGetFramebufferAttachmentParameteriv(s_workingTarget, attachment, pname, &result);

    return result;
}

void LegacyFrameBufferImplementation::attachTexture(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level) const
{
    fbo->bind(s_workingTarget);

    switch (texture->target())
    {
    case GL_TEXTURE_1D:
        glFramebufferTexture1D(s_workingTarget, attachment, texture ? texture->target() : GL_TEXTURE_1D, texture ? texture->id() : 0, level);
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
        glFramebufferTexture2D(s_workingTarget, attachment, texture ? texture->target() : GL_TEXTURE_2D, texture ? texture->id() : 0, level);
        break;
    default:
        glFramebufferTexture(s_workingTarget, attachment, texture ? texture->id() : 0, level);
        break;
    }
}

void LegacyFrameBufferImplementation::attachTextureLayer(const FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer) const
{
    fbo->bind(s_workingTarget);

    glFramebufferTextureLayer(s_workingTarget, attachment, texture ? texture->id() : 0, level, layer);
}

void LegacyFrameBufferImplementation::attachRenderBuffer(const FrameBufferObject * fbo, GLenum attachment, RenderBufferObject * renderBuffer) const
{
    fbo->bind(s_workingTarget);
    renderBuffer->bind(GL_RENDERBUFFER);

    glFramebufferRenderbuffer(s_workingTarget, attachment, GL_RENDERBUFFER, renderBuffer->id());
}

void LegacyFrameBufferImplementation::setReadBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    fbo->bind(GL_READ_FRAMEBUFFER);

    glReadBuffer(mode);
}

void LegacyFrameBufferImplementation::setDrawBuffer(const FrameBufferObject * fbo, GLenum mode) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glDrawBuffer(mode);
}

void LegacyFrameBufferImplementation::setDrawBuffers(const FrameBufferObject * fbo, GLsizei n, const GLenum * modes) const
{
    fbo->bind(GL_DRAW_FRAMEBUFFER);

    glDrawBuffers(n, modes);
}

} // namespace glo
