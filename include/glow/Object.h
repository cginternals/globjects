#pragma once

#include <GL/glew.h>

#include <glow/Error.h>

namespace glow {

class Object
{
public:
	Object();
	virtual ~Object();

	GLuint id() const;
protected:
	GLuint _id;
};

} // namespace glow
