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

	const char* typeName() const;

	void bind();
	void unbind();

	void storage(GLenum internalformat, GLsizei width, GLsizei height);
	void storageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
protected:
	static GLuint genRenderBuffer();
};

} // namespace glow
