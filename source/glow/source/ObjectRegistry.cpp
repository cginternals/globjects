#include <glow/ObjectRegistry.h>

#include <cassert>

#include <glow/Object.h>

namespace glow 
{

ObjectRegistry::ObjectRegistry()
{
}

const std::set<Object*> & ObjectRegistry::objects()
{
    return m_objects;
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
