#pragma once

#include <GL/glew.h>

#include "glow.h"

#include "glow/Referenced.h"

namespace glow
{

class GLOW_API Object : public Referenced
{
public:
	Object();
	Object(GLuint id, bool ownsGLObject = true);
	virtual ~Object();

	GLuint id() const;

	operator GLuint() const;
protected:
	GLuint m_id;
	bool m_ownsGLObject;

	bool ownsGLObject() const;
private:
	Object(const Object&);
	Object& operator=(const Object&);
};

} // namespace glow
