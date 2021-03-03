
#include "RenderbufferImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Renderbuffer.h>


using namespace gl;


namespace globjects 
{

GLuint RenderbufferImplementation_Legacy::create() const
{
    gl::GLuint id;
    glGenRenderbuffers(1, &id);

    return id;
}

void RenderbufferImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteRenderbuffers(1, &id);
}


void RenderbufferImplementation_Legacy::bind(const Renderbuffer * renderbuffer, const GLenum target) const
{
    glBindRenderbuffer(target, renderbuffer->id());
}

void RenderbufferImplementation_Legacy::unbind(const GLenum target) const
{
    glBindRenderbuffer(target, 0);
}

void RenderbufferImplementation_Legacy::storage(const Renderbuffer * renderbuffer, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height) const
{
    bind(renderbuffer, GL_RENDERBUFFER);

    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
}

void RenderbufferImplementation_Legacy::storageMultisample(const Renderbuffer * renderbuffer, gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, const gl::GLsizei height) const
{
    bind(renderbuffer, GL_RENDERBUFFER);

    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, width, height);
}

gl::GLint RenderbufferImplementation_Legacy::getParameterInt(const Renderbuffer * renderbuffer, gl::GLenum pname) const
{
    GLint value = 0;

    bind(renderbuffer, GL_RENDERBUFFER);

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, &value);

    return value;
}


} // namespace globjects
