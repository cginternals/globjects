#pragma once



#include <unordered_map>
#include <memory>

#include <glow/glow_api.h>

namespace glow
{

class ObjectRegistry;
class ExtensionRegistry;
class ImplementationRegistry;
class NamedStringRegistry;

class GLOW_API Registry
{
public:
    static long long registerCurrentContext();
    static void setContext(long long contextId);
    static void deregisterCurrentContext();

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

    static bool isContextRegistered(long long contextId);
    static void setCurrentRegistry(long long contextId);

    static Registry * s_currentRegistry;
    static std::unordered_map<long long, Registry *> s_registries;
private:
    bool m_initialized;
    std::unique_ptr<ObjectRegistry> m_objects;
    std::unique_ptr<ExtensionRegistry> m_extensions;
    std::unique_ptr<ImplementationRegistry> m_implementations;
    std::unique_ptr<NamedStringRegistry> m_namedStrings;
};

} // namespace glow
