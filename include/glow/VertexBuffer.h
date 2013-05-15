#pragma once

#include <glow/Buffer.h>

namespace glow {

class VertexArrayObject;
class VertexAttributeBinding;

class VertexBuffer : public Buffer
{
public:
	static VertexBuffer* create(VertexArrayObject* vao, GLint bindingIndex);

	GLint bindingIndex() const;
	VertexArrayObject* vao();

	void setBaseOffset(GLintptr baseOffset, GLintptr stride = 0);

	GLintptr baseOffset() const;
	GLintptr stride() const;

	void bindVertexBuffer();
private:
	VertexBuffer(VertexArrayObject* vao, GLint bindingIndex);
protected:
	VertexArrayObject* _vao;
	GLint _bindingIndex;

	GLintptr _baseOffset;
	GLintptr _stride;
};

} // namespace glow
