#include "Registry.h"

#include <glow/logging.h>

#include "contextid.h"

#include "ObjectRegistry.h"
#include "ExtensionRegistry.h"
#include "ImplementationRegistry.h"
#include "NamedStringRegistry.h"

namespace glow
{

Registry * Registry::s_currentRegistry = nullptr;
std::unordered_map<long long, Registry *> Registry::s_registries;

long long Registry::registerCurrentContext()
{
    long long contextId = getContextId();

    if (isContextRegistered(contextId))
    {
        glow::debug() << "OpenGL context " << contextId << " is already registered";

        return contextId;
    }

    setCurrentRegistry(contextId);

    return contextId;
}

void Registry::setContext(long long contextId)
{
    if (isContextRegistered(contextId))
    {
        glow::debug() << "Requesting OpenGL context " << contextId << " but it isn't registered yet";
    }

    setCurrentRegistry(contextId);
}

void Registry::deregisterCurrentContext()
{
    long long contextId = getContextId();

    if (!isContextRegistered(contextId))
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
    assert(s_currentRegistry != nullptr);

    return *s_currentRegistry;
}

bool Registry::isContextRegistered(long long contextId)
{
    return s_registries.find(contextId) != s_registries.end();
}

void Registry::setCurrentRegistry(long long contextId)
{
    auto it = s_registries.find(contextId);

    if (it != s_registries.end())
    {
        s_currentRegistry = it->second;
    }
    else
    {
        Registry * registry = new Registry();
        s_registries[contextId] = registry;

        s_currentRegistry = registry;
        registry->initialize();
    }
}

Registry::Registry()
: m_initialized(false)
{
}

Registry::~Registry()
{
}

void Registry::initialize()
{
    m_objects.reset(new ObjectRegistry);
    m_extensions.reset(new ExtensionRegistry);
    m_namedStrings.reset(new NamedStringRegistry);
    m_implementations.reset(new ImplementationRegistry);

    m_initialized = true;
}

bool Registry::isInitialized() const
{
    return m_initialized;
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
