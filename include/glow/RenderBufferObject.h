#pragma once

#include <glow/glow.h>
#include <glow/Object.h>

// http://www.opengl.org/wiki/Renderbuffer_Objects

namespace glow {

class GLOW_API RenderBufferObject : public Object
{
public:
	RenderBufferObject();
	virtual ~RenderBufferObject();

	virtual void accept(ObjectVisitor& visitor);

	void bind();
	void unbind();

	void storage(GLenum internalformat, GLsizei width, GLsizei height);
	void storageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

	GLint getParameter(GLenum pname);
protected:
	static GLuint genRenderBuffer();
};

} // namespace glow
