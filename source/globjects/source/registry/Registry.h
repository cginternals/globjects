#pragma once

#include <unordered_map>
#include <memory>

#include <glbinding/ContextHandle.h>

namespace globjects
{

class ObjectRegistry;
class ExtensionRegistry;
class ImplementationRegistry;
class NamedStringRegistry;


class Registry
{
public:
    static void registerContext(glbinding::ContextHandle contextId);
    static void registerContext(glbinding::ContextHandle contextId, glbinding::ContextHandle sharedContextId);
    static void deregisterContext(glbinding::ContextHandle contextId);

    static void setCurrentContext(glbinding::ContextHandle contextId);

    static bool isCurrentContext(glbinding::ContextHandle contextId);

    static Registry & current();

    ObjectRegistry & objects();
    ExtensionRegistry & extensions();
    ImplementationRegistry & implementations();
    NamedStringRegistry & namedStrings();

    bool isInitialized() const;

private:
    Registry();
    Registry(Registry * sharedRegistry);
    ~Registry();

    void initialize();

    static bool isContextRegistered(glbinding::ContextHandle contextId);
    static void setCurrentRegistry(glbinding::ContextHandle contextId);

    static std::unordered_map<glbinding::ContextHandle, Registry *> s_registries;

private:
    bool m_initialized;
    std::shared_ptr<ObjectRegistry> m_objects;
    std::shared_ptr<ExtensionRegistry> m_extensions;
    std::shared_ptr<ImplementationRegistry> m_implementations;
    std::shared_ptr<NamedStringRegistry> m_namedStrings;
};

} // namespace globjects
