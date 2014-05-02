#include "Registry.h"

#include <glow/logging.h>

#include "contextid.h"

#include "ObjectRegistry.h"
#include "ExtensionRegistry.h"
#include "ImplementationRegistry.h"
#include "NamedStringRegistry.h"

namespace glow
{

Registry * Registry::s_currentRegistry;
std::unordered_map<long long, Registry *> Registry::s_registries;

long long Registry::registerCurrentContext()
{
    long long contextId = getContextId();

    if (s_registries.find(contextId) != s_registries.end())
    {
        glow::debug() << "OpenGL context " << contextId << " is already registered";

        return contextId;
    }

    Registry * registry = new Registry();
    s_registries[contextId] = registry;

    s_currentRegistry = registry;

    return contextId;
}

void Registry::setContext(long long contextId)
{
    auto it = s_registries.find(contextId);
    if (it != s_registries.end())
    {
        s_currentRegistry = it->second;
    }
    else
    {
        glow::debug() << "Requesting OpenGL context " << contextId << " but it isn't registered yet";

        Registry * registry = new Registry();
        s_registries[contextId] = registry;

        s_currentRegistry = registry;
    }
}

void Registry::deregisterCurrentContext()
{
    long long contextId = getContextId();

    if (s_registries.find(contextId) == s_registries.end())
    {
        glow::debug() << "OpenGL context " << contextId << " is not registered";

        return;
    }

    delete s_registries[contextId];

    s_registries[contextId] = nullptr;
    s_currentRegistry = nullptr;
}

Registry & Registry::current()
{
    return *s_currentRegistry;
}

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
