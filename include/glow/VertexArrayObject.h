#pragma once

#include <glow/Object.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeArray.h>
#include <glow/ref_ptr.hpp>

#include <unordered_map>
#include <string>

namespace glow {

class VertexArrayObject : public Object
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void bind();
	void unbind();

	VertexAttributeArray* createAttributeArray(const std::string& name);
	VertexAttributeArray* attributeArray(const std::string& name);

	Buffer* createElementBuffer(const std::string& name);
	Buffer* elementBuffer(const std::string& name);
protected:
	std::unordered_map<std::string, ref_ptr<VertexAttributeArray>> _attributeArrays;
	std::unordered_map<std::string, ref_ptr<Buffer>> _indexBuffers;
};

} // namespace glow
