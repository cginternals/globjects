#include <glow/Object.h>

using namespace glow;

Object::Object()
: _id(0)
{
}

Object::~Object()
{
}

GLuint Object::id() const
{
	return _id;
}
