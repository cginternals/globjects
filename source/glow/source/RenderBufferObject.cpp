#include <glow/RenderBufferObject.h>

#include <glbinding/functions.h>

#include <glow/ObjectVisitor.h>

namespace glow
{

RenderBufferObject::RenderBufferObject()
: Object(genRenderBuffer())
{
}

RenderBufferObject::~RenderBufferObject()
{
	if (ownsGLObject())
	{
		gl::glDeleteRenderbuffers(1, &m_id);
	}
}

gl::GLuint RenderBufferObject::genRenderBuffer()
{
	gl::GLuint id = 0;

	gl::glGenRenderbuffers(1, &id);

	return id;
}

void RenderBufferObject::accept(ObjectVisitor& visitor)
{
	visitor.visitRenderBufferObject(this);
}

void RenderBufferObject::bind(gl::GLenum target) const
{
    gl::glBindRenderbuffer(target, m_id);
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

} // namespace glow
