
#include <glow/ShaderCode.h>
#include <glow/Shader.h>

namespace glow
{

ShaderCode::ShaderCode(const std::string& code)
: _code(code)
{
}

const std::string& ShaderCode::source()
{
	return _code;
}

} // namespace glow
