#pragma once

#include <glow/Object.h>
#include <glow/Buffer.h>

#include <unordered_map>

namespace glow {

class VertexArrayObject : public Object
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void bind();
	void unbind();

	Buffer* addArrayBuffer(const std::string& name);
	Buffer* addElementBuffer(const std::string& name);

	Buffer* buffer(const std::string& name);
protected:
	Buffer* createBuffer(const std::string& name, GLenum target);
protected:
	std::unordered_map<std::string, Buffer*> _buffers;
};

} // namespace glow
