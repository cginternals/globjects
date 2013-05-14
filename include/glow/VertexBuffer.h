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

	VertexAttributeBinding* bindToAttribute(GLint attributeLocation);
private:
	VertexBuffer(VertexArrayObject* vao, GLint bindingIndex);
protected:
	VertexArrayObject* _vao;
	GLint _bindingIndex;
};

} // namespace glow
