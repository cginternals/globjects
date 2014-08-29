#include <globjects/RenderBufferObject.h>

#include <glbinding/gl/functions.h>

#include <globjects/ObjectVisitor.h>

#include "Resource.h"

namespace glo
{

RenderBufferObject::RenderBufferObject()
: Object(new RenderBufferObjectResource)
{
}

RenderBufferObject::~RenderBufferObject()
{
}

void RenderBufferObject::accept(ObjectVisitor& visitor)
{
	visitor.visitRenderBufferObject(this);
}

void RenderBufferObject::bind(gl::GLenum target) const
{
    gl::glBindRenderbuffer(target, id());
}

void RenderBufferObject::unbind(gl::GLenum target)
{
    gl::glBindRenderbuffer(target, 0);
}

void RenderBufferObject::storage(gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height)
{
    bind(gl::GL_RENDERBUFFER);

    gl::glRenderbufferStorage(gl::GL_RENDERBUFFER, internalformat, width, height);
}

void RenderBufferObject::storageMultisample(gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height)
{
    bind(gl::GL_RENDERBUFFER);

    gl::glRenderbufferStorageMultisample(gl::GL_RENDERBUFFER, samples, internalformat, width, height);
}

gl::GLint RenderBufferObject::getParameter(gl::GLenum pname) const
{
	gl::GLint value = 0;

    bind(gl::GL_RENDERBUFFER);

	gl::glGetRenderbufferParameteriv(gl::GL_RENDERBUFFER, pname, &value);

	return value;
}

gl::GLenum RenderBufferObject::objectType() const
{
    return gl::GL_RENDERBUFFER;
}

} // namespace glo
