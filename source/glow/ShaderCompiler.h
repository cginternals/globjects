#pragma once

#include <string>

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

    ShaderCompiler(Shader* shader);
    bool checkCompileStatus();
    bool compile();
};

} // namespace glow
