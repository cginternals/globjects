
#include <globjects/Object.h>

#include "registry/ObjectRegistry.h"
#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractObjectNameImplementation.h"

#include <globjects/Resource.h>

using namespace gl;


namespace 
{


globjects::AbstractObjectNameImplementation & nameImplementation()
{
    return globjects::ImplementationRegistry::current().objectNameImplementation();
}


} // namespace


namespace globjects
{

void Object::hintNameImplementation(const NameImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}


Object::Object(std::unique_ptr<IDResource> && resource)
    : m_resource(std::move(resource))
, m_objectLabelState(nullptr)
{
    ObjectRegistry::current().registerObject(this);
}

Object::~Object()
{
    ObjectRegistry::current().deregisterObject(this);
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

    m_resource.reset(new InvalidResource());
}


} // namespace globjects
