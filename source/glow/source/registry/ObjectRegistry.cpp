#include "ObjectRegistry.h"
#include "Registry.h"

#include <cassert>

#include <glow/Object.h>
#include <glow/FrameBufferObject.h>
#include <glow/VertexArrayObject.h>

namespace glow 
{

ObjectRegistry::ObjectRegistry()
: m_defaultFBO(nullptr)
, m_defaultVAO(nullptr)
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

FrameBufferObject * ObjectRegistry::defaultFBO()
{
    if (m_defaultFBO == nullptr)
    {
        m_defaultFBO = FrameBufferObject::fromId(0);
        m_defaultFBO->ref();
    }

    return m_defaultFBO;
}

VertexArrayObject * ObjectRegistry::defaultVAO()
{
    if (m_defaultVAO == nullptr)
    {
        m_defaultVAO = VertexArrayObject::fromId(0);
        m_defaultVAO->ref();
    }

    return m_defaultVAO;
}

} // namespace glow
