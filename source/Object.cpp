
#include <glow/Object.h>
#include <glow/Error.h>
#include <glow/ObjectRegistry.h>

namespace glow
{

Object::Object()
: m_id(0)
, m_ownsGLObject(false)
{
	registerObject();
}

Object::Object(GLuint id, bool ownsGLObject)
: m_id(id)
, m_ownsGLObject(ownsGLObject)
{
	registerObject();
}

Object::~Object()
{
	deregisterObject();
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

void Object::registerObject()
{
	IF_DEBUG(ObjectRegistry::instance().registerObject(this);)
}

void Object::deregisterObject()
{
	IF_DEBUG(ObjectRegistry::instance().deregisterObject(this);)
}

const std::string& Object::name() const
{
	return m_name;
}

void Object::setName(const std::string& name)
{
	m_name = name;
}

const ObjectProperties& Object::properties()
{
	IF_DEBUG(return m_properties;)
	IF_NDEBUG(static ObjectProperties p; return p;)
}

} // namespace glow
