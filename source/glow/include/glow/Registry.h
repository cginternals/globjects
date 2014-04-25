#pragma once

#include <GL/glew.h>

#include <unordered_map>

#include <glow/glow_api.h>

namespace glow
{

class ObjectRegistry;
class ExtensionRegistry;
class StrategyRegistry;

class GLOW_API Registry
{
public:
    static Registry & current();

    ObjectRegistry & objects();
    ExtensionRegistry & extensions();
    StrategyRegistry & strategies();
private:
    Registry();
    ~Registry();

    static std::unordered_map<long long, Registry *> s_registries;
private:
    ObjectRegistry * m_objects;
    ExtensionRegistry * m_extensions;
    StrategyRegistry * m_strategies;
};

} // namespace glow
