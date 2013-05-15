#pragma once

#include <glow/Object.h>
#include <glow/Buffer.h>
#include <glow/VertexBuffer.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/ref_ptr.hpp>

#include <unordered_map>
#include <map>
#include <string>

// http://www.opengl.org/wiki/Vertex_Array_Object

namespace glow {

class VertexArrayObject : public Object
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void bind();
	void unbind();

	VertexBuffer* createVertexBuffer(const std::string& name);
	VertexBuffer* vertexBuffer(const std::string& name);

	Buffer* createElementBuffer(const std::string& name);
	Buffer* elementBuffer(const std::string& name);

	VertexAttributeBinding* binding(GLint attributeLocation);
protected:
	GLuint _nextBindingIndex;
	std::unordered_map<std::string, ref_ptr<VertexBuffer>> _vertexBuffers;
	std::unordered_map<std::string, ref_ptr<Buffer>> _indexBuffers;

	std::map<GLint, ref_ptr<VertexAttributeBinding>> _bindings;
};

} // namespace glow
