#pragma once

#include <glow/Object.h>
#include <glow/Buffer.h>
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

	Buffer* addArrayBuffer(const std::string& name);
	Buffer* addElementBuffer(const std::string& name);

	Buffer* buffer(const std::string& name);
protected:
	Buffer* createBuffer(const std::string& name, GLenum target);
protected:
	std::unordered_map<std::string, ref_ptr<Buffer>> _buffers;
};

} // namespace glow
