#pragma once

#include "declspec.h"

#include <glow/VertexBuffer.h>

namespace glow {

class VertexArrayObject;
class VertexAttributeBindingImplementation;

class GLOW_API VertexAttributeBinding : public Referenced
{
public:
	VertexAttributeBinding(VertexArrayObject* vao, GLint attributeIndex);
	~VertexAttributeBinding();

	void setBuffer(VertexBuffer* vbo, GLint baseoffset, GLint stride);

	void setFormat(GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLuint relativeoffset = 0);
	void setIFormat(GLint size, GLenum type, GLuint relativeoffset = 0);
	void setLFormat(GLint size, GLuint relativeoffset = 0);

	void enable();
	void disable();

	GLint attributeIndex() const;
	VertexArrayObject* vao() const;
	VertexBuffer* vbo() const;
protected:
	GLint _attributeIndex;
	VertexArrayObject* _vao;
	VertexBuffer* _vbo;

	VertexAttributeBindingImplementation* _implementation;
};

// GL version specific implementations

class VertexAttributeBindingImplementation
{
public:
	VertexAttributeBindingImplementation(VertexAttributeBinding* binding);
	virtual ~VertexAttributeBindingImplementation();

	virtual void bindBuffer(VertexBuffer* vbo, GLint baseoffset, GLint stride) = 0;

	virtual void setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) = 0;
	virtual void setIFormat(GLint size, GLenum type, GLuint relativeoffset) = 0;
	virtual void setLFormat(GLint size, GLenum type, GLuint relativeoffset) = 0;
protected:
	VertexAttributeBinding* _binding;
};

class VertexAttributeBinding_GL_4_3 : public VertexAttributeBindingImplementation
{
public:
	VertexAttributeBinding_GL_4_3(VertexAttributeBinding* binding);

	virtual void bindBuffer(VertexBuffer* vbo, GLint baseoffset, GLint stride);

	virtual void setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
	virtual void setIFormat(GLint size, GLenum type, GLuint relativeoffset);
	virtual void setLFormat(GLint size, GLenum type, GLuint relativeoffset);
};

class VertexAttributeBinding_GL_3_2 : public VertexAttributeBindingImplementation
{
public:
	VertexAttributeBinding_GL_3_2(VertexAttributeBinding* binding);

	virtual void bindBuffer(VertexBuffer* vbo, GLint baseoffset, GLint stride);

	virtual void setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
	virtual void setIFormat(GLint size, GLenum type, GLuint relativeoffset);
	virtual void setLFormat(GLint size, GLenum type, GLuint relativeoffset);
protected:
	struct Format
	{
		enum Method {
			O = 0,
			I = 1,
			L = 2
		};

		Format();
		Format(Method method, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);

		Method method;

		GLint size;
		GLenum type;
		GLboolean normalized;
		GLuint relativeoffset;

		bool initialized;
	};

	Format _format;
	GLint _baseoffset;
	GLint _stride;

	void bindIndex();
};

} // namespace glow
