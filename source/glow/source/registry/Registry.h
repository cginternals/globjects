#pragma once

#include <unordered_map>
#include <memory>

#include <glow/glow_api.h>
#include <glbinding/ContextId.h>

namespace glow
{

class ObjectRegistry;
class ExtensionRegistry;
class ImplementationRegistry;
class NamedStringRegistry;

class GLOW_API Registry
{
public:
    static void registerContext(glbinding::ContextId contextId);
    static void deregisterContext(glbinding::ContextId contextId);

    static void setCurrentContext(glbinding::ContextId contextId);

    static Registry & current();

    ObjectRegistry & objects();
    ExtensionRegistry & extensions();
    ImplementationRegistry & implementations();
    NamedStringRegistry & namedStrings();

    bool isInitialized() const;
private:
    Registry();
    ~Registry();

    void initialize();

    static bool isContextRegistered(glbinding::ContextId contextId);
    static void setCurrentRegistry(glbinding::ContextId contextId);

    static Registry * s_currentRegistry;
    static std::unordered_map<glbinding::ContextId, Registry *> s_registries;
private:
    bool m_initialized;
    std::unique_ptr<ObjectRegistry> m_objects;
    std::unique_ptr<ExtensionRegistry> m_extensions;
    std::unique_ptr<ImplementationRegistry> m_implementations;
    std::unique_ptr<NamedStringRegistry> m_namedStrings;
};

} // namespace glow
