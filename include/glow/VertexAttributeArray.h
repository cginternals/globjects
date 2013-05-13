#pragma once

#include <glow/Buffer.h>

namespace glow {

class VertexArrayObject;

class VertexAttributeArray : public Buffer
{
public:
	VertexAttributeArray(VertexArrayObject* vao);

	void bindToIndex(GLuint index, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid* offset = nullptr);
	void bindToIndexI(GLuint index, GLsizei stride = 0, const GLvoid* offset = nullptr);
	void bindToIndexL(GLuint index, GLsizei stride = 0, const GLvoid* offset = nullptr);

	void enable();
	void disable();
protected:
	VertexArrayObject* _vao;
	GLint _index;
};

} // namespace glow
