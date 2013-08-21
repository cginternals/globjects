#pragma once

#include <GL/glew.h>

#include <glow/Referenced.h>

namespace glow 
{

class AbstractArray : public Referenced
{
public:
	virtual const void * rawData() const = 0;
	virtual GLint rawSize() const = 0;
};

} // namespace glow
