#pragma once

#include <unordered_map>
#include <string>

#include <glowutils/glowutils.h>

namespace glow
{

class ShaderFile;

class GLOWUTILS_API ShaderFileRegistry
{
public:
    static ShaderFileRegistry& instance();

    virtual ~ShaderFileRegistry();

    bool contains(const std::string& filename) const;
    ShaderFile * obtain(const std::string& filename);

    void registerFile(ShaderFile * file);
    void deregisterFile(ShaderFile * file);

    void reloadAll();
protected:
    ShaderFileRegistry();

    std::unordered_map<std::string, ShaderFile*> m_registeredFiles;
    static ShaderFileRegistry s_registry;
};

} // namespace glow
