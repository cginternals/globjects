#include <glow/ShaderSource.h>
#include <glow/Shader.h>

using namespace glow;


void ShaderSource::addTo(Shader* shader)
{
	_shaders.insert(shader);
}

void ShaderSource::removeFrom(Shader* shader)
{
	_shaders.erase(shader);
}

void ShaderSource::updateShaders()
{
	for (Shader* shader: _shaders)
	{
		shader->sourceChanged();
	}
}


ShaderCode::ShaderCode(const std::string& code)
: _code(code)
{
}

const std::string& ShaderCode::source()
{
	return _code;
}
