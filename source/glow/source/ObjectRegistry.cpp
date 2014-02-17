#include <glow/ObjectRegistry.h>

#include <cassert>

#include <glow/Object.h>

namespace glow 
{

std::set<Object*> ObjectRegistry::s_objects;

const std::set<Object*> & ObjectRegistry::objects()
{
	return s_objects;
}

void ObjectRegistry::registerObject(Object* object)
{
    assert(object != nullptr);

	if (object->id() == 0)
        return;

	s_objects.insert(object);
}

void ObjectRegistry::deregisterObject(Object* object)
{
    assert(object != nullptr);

    if (object->id() == 0)
        return;

    s_objects.erase(object);
}

} // namespace glow
