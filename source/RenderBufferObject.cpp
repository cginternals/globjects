
#include <glow/RenderBufferObject.h>

#include <glow/Error.h>

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
		glDeleteRenderbuffers(1, &m_id);
		CheckGLError();
	}
}

GLuint RenderBufferObject::genRenderBuffer()
{
	GLuint id = 0;

	glGenRenderbuffers(1, &id);
	CheckGLError();

	return id;
}

const char* RenderBufferObject::typeName() const
{
	return "RenderBufferObject";
}

void RenderBufferObject::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	CheckGLError();
}

void RenderBufferObject::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	CheckGLError();
}

void RenderBufferObject::storage(GLenum internalformat, GLsizei width, GLsizei height)
{
	bind();

	glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	CheckGLError();
}

void RenderBufferObject::storageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	bind();

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, width, height);
	CheckGLError();
}

} // namespace glow
