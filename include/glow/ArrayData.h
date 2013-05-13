#pragma once

#include <GL/glew.h>
#include <glow/Referenced.h>

namespace glow {

class ArrayData : public Referenced
{
public:
	virtual void* rawData() = 0;
	virtual GLenum type() = 0;
	virtual GLint elementSize() = 0;
	virtual GLint byteSize() = 0;
};

} // namespace glow
