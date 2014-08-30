#include <globjects/Renderbuffer.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/ObjectVisitor.h>

#include "Resource.h"

namespace glo
{

Renderbuffer::Renderbuffer()
: Object(new RenderBufferObjectResource)
{
}

Renderbuffer::~Renderbuffer()
{
}

void Renderbuffer::accept(ObjectVisitor& visitor)
{
	visitor.visitRenderBufferObject(this);
}

void Renderbuffer::bind() const
{
    bind(gl::GL_RENDERBUFFER);
}

void Renderbuffer::unbind()
{
    unbind(gl::GL_RENDERBUFFER);
}

void Renderbuffer::bind(gl::GLenum target) const
{
    gl::glBindRenderbuffer(target, id());
}

void Renderbuffer::unbind(gl::GLenum target)
{
    gl::glBindRenderbuffer(target, 0);
}

void Renderbuffer::storage(gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height)
{
    bind(gl::GL_RENDERBUFFER);

    gl::glRenderbufferStorage(gl::GL_RENDERBUFFER, internalformat, width, height);
}

void Renderbuffer::storageMultisample(gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height)
{
    bind(gl::GL_RENDERBUFFER);

    gl::glRenderbufferStorageMultisample(gl::GL_RENDERBUFFER, samples, internalformat, width, height);
}

gl::GLint Renderbuffer::getParameter(gl::GLenum pname) const
{
	gl::GLint value = 0;

    bind(gl::GL_RENDERBUFFER);

	gl::glGetRenderbufferParameteriv(gl::GL_RENDERBUFFER, pname, &value);

	return value;
}

gl::GLenum Renderbuffer::objectType() const
{
    return gl::GL_RENDERBUFFER;
}

} // namespace glo
