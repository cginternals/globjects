#include "ObjectRegistry.h"
#include "Registry.h"

#include <cassert>

#include <glow/Object.h>

namespace glow 
{

ObjectRegistry::ObjectRegistry()
{
}

ObjectRegistry & ObjectRegistry::current()
{
    return Registry::current().objects();
}

const std::set<Object*> & ObjectRegistry::objects()
{
    return m_objects;
}

std::set<Object *>::iterator ObjectRegistry::begin()
{
    return m_objects.begin();
}

std::set<Object *>::iterator ObjectRegistry::end()
{
    return m_objects.end();
}

void ObjectRegistry::registerObject(Object * object)
{
    assert(object != nullptr);

	if (object->id() == 0)
        return;

    m_objects.insert(object);
}

void ObjectRegistry::deregisterObject(Object * object)
{
    assert(object != nullptr);

    if (object->id() == 0)
        return;

    m_objects.erase(object);
}

} // namespace glow
