#include <globjects/Renderbuffer.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/ObjectVisitor.h>

#include "Resource.h"


using namespace gl;

namespace globjects
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
    bind(GL_RENDERBUFFER);
}

void Renderbuffer::unbind()
{
    unbind(GL_RENDERBUFFER);
}

void Renderbuffer::bind(const GLenum target) const
{
    glBindRenderbuffer(target, id());
}

void Renderbuffer::unbind(const GLenum target)
{
    glBindRenderbuffer(target, 0);
}

void Renderbuffer::storage(const GLenum internalformat, const GLsizei width, const GLsizei height)
{
    bind(GL_RENDERBUFFER);

    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
}

void Renderbuffer::storageMultisample(const GLsizei samples, const GLenum internalformat, const GLsizei width, const GLsizei height)
{
    bind(GL_RENDERBUFFER);

    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, width, height);
}

GLint Renderbuffer::getParameter(const GLenum pname) const
{
	GLint value = 0;

    bind(GL_RENDERBUFFER);

	glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, &value);

	return value;
}

GLenum Renderbuffer::objectType() const
{
    return GL_RENDERBUFFER;
}

} // namespace globjects
