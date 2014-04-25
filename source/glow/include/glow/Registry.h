#pragma once

#include <GL/glew.h>

#include <unordered_map>

#include <glow/glow_api.h>

namespace glow
{

class ObjectRegistry;
class ExtensionRegistry;

class GLOW_API Registry
{
public:
    static Registry & current();

    ObjectRegistry & objects();
    ExtensionRegistry & extensions();
private:
    Registry();
    ~Registry();

    static std::unordered_map<long long, Registry *> s_registries;
private:
    ObjectRegistry * m_objects;
    ExtensionRegistry * m_extensions;
};

} // namespace glow
