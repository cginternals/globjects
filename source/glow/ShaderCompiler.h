#pragma once

#include <string>
#include <set>

#include <glow/glow.h>

namespace glow 
{
class Shader;

class ShaderCompiler
{
public:
    virtual ~ShaderCompiler();

    static bool compile(Shader* shader);
protected:
    Shader* m_shader;
    std::set<std::string> m_defines;

    ShaderCompiler(Shader* shader);
    bool checkCompileStatus();
    bool compile();
    std::string resolveIncludes(const std::string& source, bool dropVersion = false);
};

} // namespace glow
