#pragma once

#include <GL/glew.h>

#include <unordered_map>

#include <glow/glow_api.h>

namespace glow
{

class ObjectRegistry;
class ExtensionRegistry;
class BehaviorRegistry;
class NamedStringRegistry;

class GLOW_API Registry
{
public:
    static Registry & current();

    ObjectRegistry & objects();
    ExtensionRegistry & extensions();
    BehaviorRegistry & behaviors();
    NamedStringRegistry & namedStrings();
private:
    Registry();
    ~Registry();

    static std::unordered_map<long long, Registry *> s_registries;
private:
    ObjectRegistry * m_objects;
    ExtensionRegistry * m_extensions;
    BehaviorRegistry * m_behaviors;
    NamedStringRegistry * m_namedStrings;
};

} // namespace glow
