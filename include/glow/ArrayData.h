#pragma once

#include <GL/glew.h>
#include <glow/Referenced.h>

namespace glow {

class ArrayData : public Referenced
{
public:
	virtual const void* rawData() const = 0;
	virtual GLenum type() const = 0;
	virtual GLint byteSize() const = 0;
	virtual GLint elementCount() const = 0;
	virtual GLint typeCountPerElement() const = 0;
	virtual GLint sizeofElement() const = 0;
};

} // namespace glow
