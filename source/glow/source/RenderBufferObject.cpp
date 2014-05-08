#include <glow/RenderBufferObject.h>

#include <glbinding/functions.h>

#include <glow/Error.h>
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
		gl::DeleteRenderbuffers(1, &m_id);
	}
}

gl::GLuint RenderBufferObject::genRenderBuffer()
{
	gl::GLuint id = 0;

	gl::GenRenderbuffers(1, &id);

	return id;
}

void RenderBufferObject::accept(ObjectVisitor& visitor)
{
	visitor.visitRenderBufferObject(this);
}

void RenderBufferObject::bind(gl::GLenum target) const
{
    gl::BindRenderbuffer(target, m_id);
}

void RenderBufferObject::unbind(gl::GLenum target)
{
    gl::BindRenderbuffer(target, 0);
}

void RenderBufferObject::storage(gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height)
{
    bind(gl::RENDERBUFFER);

    gl::RenderbufferStorage(gl::RENDERBUFFER, internalformat, width, height);
}

void RenderBufferObject::storageMultisample(gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height)
{
    bind(gl::RENDERBUFFER);

    gl::RenderbufferStorageMultisample(gl::RENDERBUFFER, samples, internalformat, width, height);
}

gl::GLint RenderBufferObject::getParameter(gl::GLenum pname) const
{
	gl::GLint value = 0;

    bind(gl::RENDERBUFFER);

	gl::GetRenderbufferParameteriv(gl::RENDERBUFFER, pname, &value);

	return value;
}

} // namespace glow
