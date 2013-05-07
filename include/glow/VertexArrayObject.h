#pragma once

#include <glow/Object.h>
#include <glow/Buffer.h>

#include <vector>

namespace glow {

class VertexArrayObject : public Object
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void bind();
	void unbind();

	Buffer* addArrayBuffer();
	Buffer* addElementBuffer();
protected:
	Buffer* createBuffer(GLenum target);
protected:
	std::vector<Buffer*> _buffers;
};

} // namespace glow
