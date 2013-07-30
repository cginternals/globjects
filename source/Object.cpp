#include <glow/Object.h>

using namespace glow;

Object::Object()
: m_id(0)
, _ownsGLObject(false)
{
}

Object::Object(GLuint id, bool ownsGLObject)
: m_id(id)
, _ownsGLObject(ownsGLObject)
{
}

Object::~Object()
{
}

GLuint Object::id() const
{
	return m_id;
}

Object::operator GLuint() const
{
	return m_id;
}

bool Object::ownsGLObject() const
{
    return _ownsGLObject && m_id>0;
}
