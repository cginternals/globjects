#include "Registry.h"

#include "contextid.h"

#include "ObjectRegistry.h"
#include "ExtensionRegistry.h"
#include "ImplementationRegistry.h"
#include "NamedStringRegistry.h"

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
, m_implementations(new ImplementationRegistry)
, m_namedStrings(new NamedStringRegistry)
{
}

Registry::~Registry()
{
    delete m_objects;
    delete m_extensions;
    delete m_implementations;
    delete m_namedStrings;
}

ObjectRegistry & Registry::objects()
{
    return *m_objects;
}

ExtensionRegistry & Registry::extensions()
{
    return *m_extensions;
}

ImplementationRegistry & Registry::implementations()
{
    return *m_implementations;
}

NamedStringRegistry & Registry::namedStrings()
{
    return *m_namedStrings;
}

} // namespace glow
