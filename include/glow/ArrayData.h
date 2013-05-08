#pragma once

#include <GL/glew.h>

namespace glow {

class ArrayData
{
public:
	virtual void* rawData() = 0;
	virtual GLenum type() = 0;
	virtual GLint elementSize() = 0;
	virtual GLint byteSize() = 0;
};

} // namespace glow
