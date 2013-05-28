#include <glow/ShaderSource.h>
#include <glow/Shader.h>

using namespace glow;


ShaderCode::ShaderCode(const std::string& code)
: _code(code)
{
}

const std::string& ShaderCode::source()
{
	return _code;
}
