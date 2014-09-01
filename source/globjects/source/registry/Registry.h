#pragma once

#include <unordered_map>
#include <memory>

#include <globjects/globjects_api.h>
#include <glbinding/ContextHandle.h>

namespace glo
{

class ObjectRegistry;
class ExtensionRegistry;
class ImplementationRegistry;
class NamedStringRegistry;

class GLOBJECTS_API Registry
{
public:
    static void registerContext(glbinding::ContextHandle contextId);
    static void deregisterContext(glbinding::ContextHandle contextId);

    static void setCurrentContext(glbinding::ContextHandle contextId);

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

    static bool isContextRegistered(glbinding::ContextHandle contextId);
    static void setCurrentRegistry(glbinding::ContextHandle contextId);

    static std::unordered_map<glbinding::ContextHandle, Registry *> s_registries;
private:
    bool m_initialized;
    std::unique_ptr<ObjectRegistry> m_objects;
    std::unique_ptr<ExtensionRegistry> m_extensions;
    std::unique_ptr<ImplementationRegistry> m_implementations;
    std::unique_ptr<NamedStringRegistry> m_namedStrings;
};

} // namespace glo
