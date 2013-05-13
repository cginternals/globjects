#pragma once

#include <glow/Referenced.h>
#include <glow/Buffer.h>


namespace glow {

class Program;

class VertexAttribute : public Referenced
{
public:
	VertexAttribute(GLint index, Program* program);
	~VertexAttribute();

	GLint index() const;
	bool isValid() const;

	Program* program();
	Buffer* buffer();

	void enable();
	void disable();
	void setEnabled(bool state);

	//void setPointer(GLuint index, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid* pointer = nullptr);
	void setBuffer(Buffer* buffer, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid* pointer = nullptr);
protected:
	Program* _program;
	GLint _index;
	ref_ptr<Buffer> _buffer;
};

} // namespace glow
