#pragma once

#include <GL/glew.h>

#include <glow/Referenced.h>

namespace glow {

class Object : public Referenced
{
public:
	Object();
	virtual ~Object();

	GLuint id() const;
protected:
	GLuint _id;
private:
	Object(const Object&);
	Object& operator=(const Object&);
};

} // namespace glow
