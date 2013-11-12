
#include <glow/ShaderSource.h>

namespace glow
{

std::string ShaderSource::shortInfo() const
{
    return "";
}

std::set<Shader*> ShaderSource::requiredShaders() const
{
    return std::set<Shader*>();
}

} // namespace glow
