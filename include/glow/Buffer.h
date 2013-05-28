#pragma once

#include <glow/declspec.h>

#include <glow/Object.h>
#include <glow/ArrayData.h>

namespace glow
{

class GLOW_API Buffer : public Object
{
public:
	Buffer();
	Buffer(GLenum target);
	~Buffer();

	void bind();
	void bind(GLenum target);
	void unbind();

	void setData(const ArrayData& data, GLenum usage = GL_STATIC_DRAW);
	void setData(GLsizei size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
	
	// mapping
	template<class ReturnType>
	ReturnType map(unsigned size, GLenum access = GL_READ_ONLY) {
		return map<ReturnType>(_target, size, access);
	};
	
	template<class ReturnType>
	ReturnType map(GLenum target, unsigned size, GLenum access = GL_READ_ONLY) {
		bind(target);
		
		void* data = glMapBuffer(_target, access);
		
		if (data)
		{
			return ReturnType(data, size);
		}
		else
		{
			return ReturnType();
		}
	};
	
	void unmap();

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
