#pragma once

#include <glow/Object.h>
#include <glow/ArrayData.h>


namespace glow {

class Buffer : public Object
{
public:
	Buffer();
	Buffer(GLenum target);
	~Buffer();

	void bind();
	void bind(GLenum target);
	void unbind();

	virtual void setData(ArrayData* data, GLenum usage);
	ArrayData* data();

	// drawing
	void drawArrays(GLenum mode, GLint first = 0, GLsizei count = -1);
	void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices = nullptr);

	// indexed buffer binding
	void bindBase(GLenum target, GLuint index);
	void bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size);
protected:
	GLenum _target;
	ref_ptr<ArrayData> _data;
};

} // namespace glow

