
#include "AbstractObjectNameImplementation.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>

#include <globjects/Object.h>
#include <globjects/Sync.h>

#include "ObjectNameImplementation_KHR_debug.h"
#include "ObjectNameImplementation_Legacy.h"

using namespace gl;

namespace globjects {

AbstractObjectNameImplementation::AbstractObjectNameImplementation()
{
}

AbstractObjectNameImplementation::~AbstractObjectNameImplementation()
{
}

AbstractObjectNameImplementation * AbstractObjectNameImplementation::get(const Object::NameImplementation impl)
{
    if (impl == Object::NameImplementation::DebugKHR
     && hasExtension(GLextension::GL_KHR_debug))
    {
        return new ObjectNameImplementation_KHR_debug();
    }
    else
    {
        return new ObjectNameImplementation_Legacy();
    }
}

void * AbstractObjectNameImplementation::objectLabelState(const Object * object) const
{
    return object->m_objectLabelState;
}

void * AbstractObjectNameImplementation::objectLabelState(const Sync * sync) const
{
    return sync->m_objectLabelState;
}

void AbstractObjectNameImplementation::setObjectLabelState(const Object * object, void * state) const
{
    object->m_objectLabelState = state;
}

void AbstractObjectNameImplementation::setObjectLabelState(const Sync * sync, void * state) const
{
    sync->m_objectLabelState = state;
}

} // namespace globjects
