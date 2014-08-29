#include <globjects/Object.h>

#include "registry/ObjectRegistry.h"
#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractObjectNameImplementation.h"

#include "Resource.h"

namespace {
    glo::AbstractObjectNameImplementation & nameImplementation()
    {
        return glo::ImplementationRegistry::current().objectNameImplementation();
    }
}

namespace glo
{

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

gl::GLuint Object::id() const
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

} // namespace glo
