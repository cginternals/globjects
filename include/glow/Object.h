#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

namespace glow
{

class GLOW_API Object : public Referenced
{
public:
	Object();
	Object(GLuint id, bool ownsGLObject = true);
	virtual ~Object();

	virtual const char* typeName() const = 0;

	GLuint id() const;
	operator GLuint() const;

	bool ownsGLObject() const;
public:
	bool isTransformFeedback() const;
protected:
	GLuint m_id;
	bool m_ownsGLObject;
private:
	Object(const Object &);
	Object & operator=(const Object &);
};

} // namespace glow
