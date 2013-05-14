#pragma once

#include <glow/VertexBuffer.h>

namespace glow {

class VertexArrayObject;

class VertexAttributeBinding : public Referenced
{
public:
	VertexAttributeBinding(VertexArrayObject* vao, GLint index);

	void setBuffer(VertexBuffer* vbo);

	void setFormat(GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid* offset = nullptr);
	void setFormatL(GLsizei stride = 0, const GLvoid* offset = nullptr);
	void setFormatI(GLsizei stride = 0, const GLvoid* offset = nullptr);

	void enable();
	void disable();
protected:
	struct Format
	{
		enum Type {
			Normal = 0,
			I = 1,
			L = 2
		};

		Format();
		Format(Type type, GLboolean normalized, GLsizei stride , const GLvoid* offset);

		Type type;
		GLboolean normalized;
		GLsizei stride;
		const GLvoid* offset;
	};

	GLint _index;
	Format _format;
	VertexArrayObject* _vao;
	VertexBuffer* _vbo;

	void bindIndex();
};

} // namespace glow
