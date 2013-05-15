#pragma once

#include <glow/Object.h>

// http://www.opengl.org/wiki/Renderbuffer_Objects

namespace glow {

class RenderBufferObject : public Object
{
public:
	RenderBufferObject();
	~RenderBufferObject();

	void bind();
	void unbind();

	void storage(GLenum internalformat, GLsizei width, GLsizei height);
	void storageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
};

} // namespace glow
