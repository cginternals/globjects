#pragma once

#include <string>
#include <unordered_map>

#include <glowutils/glowutils.h>

namespace glow
{

class Shader;

class GLOWUTILS_API ShaderIncludeRegistry
{
public:
    virtual ~ShaderIncludeRegistry();

    bool contains(const std::string& filename) const;

    static ShaderIncludeRegistry& instance();
protected:
    ShaderIncludeRegistry();

    static ShaderIncludeRegistry s_registry;
    std::unordered_map<std::string, Shader*> m_registeredShaders;

};

} // namespace glow
