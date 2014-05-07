#include <glow/Object.h>

#include "registry/ObjectRegistry.h"

namespace glow
{

Object::Object(gl::GLuint id, bool takeOwnership)
: m_id(id)
, m_ownsGLObject(takeOwnership)
{
    ObjectRegistry::current().registerObject(this);
}

Object::~Object()
{
    ObjectRegistry::current().deregisterObject(this);
}

gl::GLuint Object::id() const
{
	return m_id;
}

bool Object::ownsGLObject() const
{
    return m_ownsGLObject && m_id>0;
}

void Object::takeOwnership()
{
    m_ownsGLObject = true;
}

void Object::releaseOwnership()
{
    m_ownsGLObject = false;
}

const std::string& Object::name() const
{
	return m_name;
}

void Object::setName(const std::string & name)
{
	m_name = name;
}

bool Object::hasName() const
{
    return !m_name.empty();
}

} // namespace glow
