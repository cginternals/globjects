#pragma once

#include <GL/glew.h>

namespace glow 
{

class AbstractArray
{
public:
	virtual const void * rawData() const = 0;
	virtual GLint rawSize() const = 0;
};

} // namespace glow
