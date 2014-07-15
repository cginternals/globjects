#include <glow/Object.h>

#include "registry/ObjectRegistry.h"

#include "Resource.h"

namespace glow
{

Object::Object(IDResource * resource)
: m_resource(resource)
{
    ObjectRegistry::current().registerObject(this);
}

Object::~Object()
{
    ObjectRegistry::current().deregisterObject(this);
    delete m_resource;
}

gl::GLuint Object::id() const
{
    return m_resource->id();
}

bool Object::isDefault() const
{
    return id() == 0;
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
