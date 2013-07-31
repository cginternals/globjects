
#include <glow/Object.h>

namespace glow
{

Object::Object()
: m_id(0)
, m_ownsGLObject(false)
{
}

Object::Object(GLuint id, bool ownsGLObject)
: m_id(id)
, m_ownsGLObject(ownsGLObject)
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
    return m_ownsGLObject && m_id>0;
}

} // namespace glow
