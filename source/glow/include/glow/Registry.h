#pragma once

#include <GL/glew.h>

#include <glow/glow_api.h>

#include <unordered_map>

namespace glow
{

class ObjectRegistry;

class GLOW_API Registry
{
public:
    static Registry & current();

    ObjectRegistry & objects();
private:
    Registry();
    ~Registry();

    static std::unordered_map<long long, Registry *> s_registries;
private:
    ObjectRegistry * m_objects;
};

} // namespace glow
