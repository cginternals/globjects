#pragma once

#include <glow/Object.h>
#include <glow/Buffer.h>
#include <glow/ref_ptr.hpp>

#include <unordered_map>
#include <string>

namespace glow {

class VertexArrayObject;

class VertexAttributeArray : public Buffer
{
public:
	VertexAttributeArray(GLuint index, VertexArrayObject* vao);

	void setData(ArrayData* data, GLenum usage, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid* offset = nullptr);
	void setDataI(ArrayData* data, GLenum usage, GLsizei stride = 0, const GLvoid* offset = nullptr);
	void setDataL(ArrayData* data, GLenum usage, GLsizei stride = 0, const GLvoid* offset = nullptr);

	void enable();
	void disable();
protected:
	GLuint _index;
	VertexArrayObject* _vao;
};

class VertexArrayObject : public Object
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void bind();
	void unbind();

	VertexAttributeArray* createAttributeArray(const std::string& name, GLuint index);
	VertexAttributeArray* attributeArray(const std::string& name);

	Buffer* createElementBuffer(const std::string& name);
	Buffer* elementBuffer(const std::string& name);
protected:
	std::unordered_map<std::string, ref_ptr<VertexAttributeArray>> _attributeArrays;
	std::unordered_map<std::string, ref_ptr<Buffer>> _indexBuffers;
};

} // namespace glow
