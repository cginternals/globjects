
#include <glow/Shader.h>
#include <glow/ShaderCode.h>

namespace glow
{

ShaderCode::ShaderCode(const std::string & source)
:    m_source(source)
{
}

const std::string & ShaderCode::source()
{
	return m_source;
}

} // namespace glow
