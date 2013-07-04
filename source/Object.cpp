#include <glow/Object.h>

using namespace glow;

Object::Object()
: _id(0)
, _ownsGLObject(false)
{
}

Object::Object(GLuint id, bool ownsGLObject)
: _id(id)
, _ownsGLObject(ownsGLObject)
{
}

Object::~Object()
{
}

GLuint Object::id() const
{
	return _id;
}

Object::operator GLuint() const
{
	return _id;
}

bool Object::ownsGLObject() const
{
    return _ownsGLObject && _id>0;
}
