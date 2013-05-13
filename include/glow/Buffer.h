#pragma once

#include <glow/Object.h>
#include <glow/ArrayData.h>


namespace glow {

class Buffer : public Object
{
public:
	Buffer(GLenum target);
	~Buffer();

	void bind();
	void unbind();

	virtual void setData(ArrayData* data, GLenum usage);
	ArrayData* data();

	void drawArrays(GLenum mode, GLint first = 0, GLsizei count = -1);
	void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices = nullptr);
protected:
	GLenum _target;
	ref_ptr<ArrayData> _data;
};

} // namespace glow
