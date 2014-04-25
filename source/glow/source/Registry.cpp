#include <glow/Registry.h>

#include "contextid.h"

#include <glow/ObjectRegistry.h>

namespace glow
{

Registry & Registry::current()
{
    long long contextId = getContextId();

    auto it = s_registries.find(contextId);
    if (it != s_registries.end())
    {
        return *it->second;
    }
    else
    {
        Registry * registry = new Registry();
        s_registries[contextId] = registry;

        return *registry;
    }
}

std::unordered_map<long long, Registry *> Registry::s_registries;

Registry::Registry()
: m_objects(new ObjectRegistry())
{
}

Registry::~Registry()
{
}

ObjectRegistry & Registry::objects()
{
    return *m_objects;
}

} // namespace glow