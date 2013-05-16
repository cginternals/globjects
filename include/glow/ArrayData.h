#pragma once

#include <GL/glew.h>

#include "Referenced.h"

namespace glow {

class ArrayData : public Referenced
{
public:
	virtual const void* rawData() const = 0;
	virtual GLint byteSize() const = 0;
};

} // namespace glow
