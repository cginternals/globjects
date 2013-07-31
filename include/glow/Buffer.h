#pragma once

#include <glow/glow.h>

#include <glow/Object.h>
#include <glow/ArrayData.h>

namespace glow
{

class GLOW_API Buffer : public Object
{
public:
	Buffer();
	Buffer(GLenum target);
	Buffer(GLuint id, GLenum target, bool ownsGLObject = true);
	~Buffer();

	void bind();
	void bind(GLenum target);
	void unbind();

	void setData(const ArrayData& data, GLenum usage = GL_STATIC_DRAW);
	void setData(GLsizei size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);

	// mapping
	void* map(GLenum access = GL_READ_ONLY);
	void* map(GLenum target, GLenum access);
	void unmap();

//	template<typename T> T* map(GLenum access = GL_READ_ONLY) { return reinterpret_cast<T*>(map(access));}
//	template<typename T> T* map(GLenum target, GLenum access = GL_READ_ONLY) { return reinterpret_cast<T*>(map(target, access));}

	// drawing
	void drawArrays(GLenum mode, GLint first, GLsizei count);
	void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices = nullptr);

	// indexed buffer binding
	void bindBase(GLenum target, GLuint index);
	void bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size);
protected:
	GLenum _target;

	static GLuint genBuffer();
};

} // namespace glow
