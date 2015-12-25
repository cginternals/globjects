#include "Registry.h"

#include <mutex>

#include <globjects/globjects_features.h>
#include <globjects/logging.h>

#include "ObjectRegistry.h"
#include "ExtensionRegistry.h"
#include "ImplementationRegistry.h"
#include "NamedStringRegistry.h"

namespace
{
	GLOBJECTS_THREAD_LOCAL globjects::Registry * t_currentRegistry;

    std::recursive_mutex g_mutex;
}

namespace globjects
{

std::unordered_map<glbinding::ContextHandle, Registry *> Registry::s_registries;

void Registry::registerContext(glbinding::ContextHandle contextId)
{
    if (isContextRegistered(contextId))
    {
        globjects::debug() << "OpenGL context " << contextId << " is already registered";
    }

    setCurrentRegistry(contextId);
}

void Registry::registerContext(const glbinding::ContextHandle contextId, const glbinding::ContextHandle sharedContextId)
{
    if (isContextRegistered(contextId))
    {
        globjects::debug() << "OpenGL context " << contextId << " is already registered";
    }

    g_mutex.lock();
    auto it = s_registries.find(sharedContextId);
    assert(it != s_registries.end());
    g_mutex.unlock();

    Registry * registry = new Registry(it->second);

    g_mutex.lock();
    s_registries[contextId] = registry;
    g_mutex.unlock();

    t_currentRegistry = registry;
    //registry->initialize();
}

void Registry::setCurrentContext(const glbinding::ContextHandle contextId)
{
    if (!isContextRegistered(contextId))
    {
        globjects::debug() << "Requesting OpenGL context " << contextId << " but it isn't registered yet";
    }

    setCurrentRegistry(contextId);
}

bool Registry::isCurrentContext(glbinding::ContextHandle contextId)
{
    g_mutex.lock();

    const auto it = s_registries.find(contextId);
    const auto result = it != s_registries.end() && it->second == t_currentRegistry;

    g_mutex.unlock();

    return result;
}

void Registry::deregisterContext(const glbinding::ContextHandle contextId)
{
    if (!isContextRegistered(contextId))
    {
        globjects::debug() << "OpenGL context " << contextId << " is not registered";

        return;
    }

    g_mutex.lock();
    delete s_registries[contextId];
    g_mutex.unlock();

    g_mutex.lock();
    s_registries[contextId] = nullptr;
    g_mutex.unlock();

    t_currentRegistry = nullptr;
}

Registry & Registry::current()
{
    assert(t_currentRegistry != nullptr);

    return *t_currentRegistry;
}

bool Registry::isContextRegistered(const glbinding::ContextHandle contextId)
{
    g_mutex.lock();
    bool found = s_registries.find(contextId) != s_registries.end();
    g_mutex.unlock();

    return found;
}

void Registry::setCurrentRegistry(const glbinding::ContextHandle contextId)
{
    g_mutex.lock();
    auto it = s_registries.find(contextId);

    if (it != s_registries.end())
    {
        t_currentRegistry = it->second;

        g_mutex.unlock();
    }
    else
    {
        g_mutex.unlock();

        Registry * registry = new Registry();

        g_mutex.lock();
        s_registries[contextId] = registry;
        g_mutex.unlock();

        t_currentRegistry = registry;
        registry->initialize();
    }
}

Registry::Registry()
: m_initialized(false)
{
}

Registry::Registry(Registry * sharedRegistry)
: m_initialized(true)
, m_objects(sharedRegistry->m_objects)
, m_extensions(sharedRegistry->m_extensions)
, m_implementations(sharedRegistry->m_implementations)
, m_namedStrings(sharedRegistry->m_namedStrings)
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

} // namespace globjects
