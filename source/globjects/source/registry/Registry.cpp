#include "Registry.h"

#include <mutex>

#include <globjects/logging.h>

#include "ObjectRegistry.h"
#include "ExtensionRegistry.h"
#include "ImplementationRegistry.h"
#include "NamedStringRegistry.h"

namespace
{
    THREAD_LOCAL glo::Registry * t_currentRegistry;

    std::recursive_mutex mutex;
}

namespace glo
{

std::unordered_map<glbinding::ContextHandle, Registry *> Registry::s_registries;

void Registry::registerContext(glbinding::ContextHandle contextId)
{
    if (isContextRegistered(contextId))
    {
        glo::debug() << "OpenGL context " << contextId << " is already registered";
    }

    setCurrentRegistry(contextId);
}

void Registry::setCurrentContext(glbinding::ContextHandle contextId)
{
    if (!isContextRegistered(contextId))
    {
        glo::debug() << "Requesting OpenGL context " << contextId << " but it isn't registered yet";
    }

    setCurrentRegistry(contextId);
}

void Registry::deregisterContext(glbinding::ContextHandle contextId)
{
    if (!isContextRegistered(contextId))
    {
        glo::debug() << "OpenGL context " << contextId << " is not registered";

        return;
    }

    mutex.lock();
    delete s_registries[contextId];
    mutex.unlock();

    mutex.lock();
    s_registries[contextId] = nullptr;
    mutex.unlock();

    t_currentRegistry = nullptr;
}

Registry & Registry::current()
{
    assert(t_currentRegistry != nullptr);

    return *t_currentRegistry;
}

bool Registry::isContextRegistered(glbinding::ContextHandle contextId)
{
    mutex.lock();
    bool found = s_registries.find(contextId) != s_registries.end();
    mutex.unlock();

    return found;
}

void Registry::setCurrentRegistry(glbinding::ContextHandle contextId)
{
    mutex.lock();
    auto it = s_registries.find(contextId);

    if (it != s_registries.end())
    {
        t_currentRegistry = it->second;

        mutex.unlock();
    }
    else
    {
        mutex.unlock();

        Registry * registry = new Registry();

        mutex.lock();
        s_registries[contextId] = registry;
        mutex.unlock();

        t_currentRegistry = registry;
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

} // namespace glo
