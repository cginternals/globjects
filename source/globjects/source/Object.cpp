
#include <globjects/Object.h>

#include "registry/ObjectRegistry.h"
#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractObjectNameImplementation.h"

#include "Resource.h"

using namespace gl;

namespace 
{

globjects::AbstractObjectNameImplementation & nameImplementation()
{
    return globjects::ImplementationRegistry::current().objectNameImplementation();
}

}

namespace globjects
{

void Object::hintNameImplementation(const NameImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}


Object::Object(IDResource * resource)
: m_resource(resource)
, m_objectLabelState(nullptr)
{
    ObjectRegistry::current().registerObject(this);
}

Object::~Object()
{
    ObjectRegistry::current().deregisterObject(this);
    delete m_resource;
}

GLuint Object::id() const
{
    return m_resource->id();
}

bool Object::isDefault() const
{
    return id() == 0;
}

std::string Object::name() const
{
    return nameImplementation().getLabel(this);
}

void Object::setName(const std::string & name)
{
    nameImplementation().setLabel(this, name);
}

bool Object::hasName() const
{
    return nameImplementation().hasLabel(this);
}

void Object::detach()
{
    if (!m_resource)
        return;

    ObjectRegistry::current().deregisterObject(this);

    delete m_resource;
    m_resource = new InvalidResource();
}

} // namespace globjects
