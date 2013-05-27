#pragma once

#include <GL/glew.h>

#include "declspec.h"

#include "glow/Referenced.h"

namespace glow
{

class GLOW_API Object : public Referenced
{
public:
	Object();
	Object(GLuint id);
	virtual ~Object();

	GLuint id() const;

	operator GLuint() const;
protected:
	GLuint _id;
private:
	Object(const Object&);
	Object& operator=(const Object&);
};

} // namespace glow
