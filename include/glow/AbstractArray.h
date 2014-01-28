#pragma once

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow 
{

/** \brief Designed for array like classes for easy use with glow.
    
    Most OpenGL functions need a void* and the length of the memory to operate 
    on data. Both information can be provided by this class encapsulated into 
    one single object.

    The only subclass at the time is Array.
    
    \see Array
 */
class GLOW_API AbstractArray
{
public:
	virtual const void * rawData() const = 0;
	virtual GLint rawSize() const = 0;
};

} // namespace glow
