#include <glow/VertexAttribute.h>

#include <glow/Program.h>


using namespace glow;


VertexAttribute::VertexAttribute(GLint index, Program* program)
: _index(index)
, _program(program)
{
}

VertexAttribute::~VertexAttribute()
{
}

GLint VertexAttribute::index() const
{
	return _index;
}

bool VertexAttribute::isValid() const
{
	return _index>=0;
}

Program* VertexAttribute::program()
{
	return _program;
}

Buffer* VertexAttribute::buffer()
{
	return _buffer;
}

void VertexAttribute::enable()
{
	_program->use();
	glEnableVertexAttribArray(_index);
	_program->release();
}

void VertexAttribute::disable()
{
	_program->use();
	glDisableVertexAttribArray(_index);
	_program->release();
}

void VertexAttribute::setEnabled(bool state)
{
	state ? enable() : disable();
}

void VertexAttribute::setBuffer(Buffer* buffer, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	_buffer = buffer;
	_program->use();
	_buffer->bind();
	glVertexAttribPointer(_index, _buffer->data()->elementSize(), _buffer->data()->type(), normalized, stride, pointer);
	_buffer->unbind();
	_program->release();
}
