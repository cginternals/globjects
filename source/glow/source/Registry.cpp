#include <glow/Registry.h>

#include "contextid.h"

#include <glow/ObjectRegistry.h>
#include <glow/ExtensionRegistry.h>
#include <glow/StrategyRegistry.h>

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
: m_objects(new ObjectRegistry)
, m_extensions(new ExtensionRegistry)
, m_strategies(new StrategyRegistry)
{
}

Registry::~Registry()
{
    delete m_objects;
    delete m_extensions;
}

ObjectRegistry & Registry::objects()
{
    return *m_objects;
}

ExtensionRegistry & Registry::extensions()
{
    return *m_extensions;
}

StrategyRegistry & Registry::strategies()
{
    return *m_strategies;
}

} // namespace glow
