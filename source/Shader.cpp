
#include <vector>

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/Log.h>
#include <glow/ShaderCode.h>

namespace glow
{

Shader::Shader(GLenum type)
: Object(createShader(type))
, _type(type)
, _source(nullptr)
, _compiled(false)
{
}

Shader::~Shader()
{
	if (_source)
	{
		_source->deregisterListener(this);
	}
	if (m_id) glDeleteShader(m_id);
}

GLuint Shader::createShader(GLenum type)
{
	return glCreateShader(type);
}

Shader* Shader::fromFile(GLenum type, const std::string& filename)
{
	Shader* shader = new Shader(type);
	shader->setSource(new ShaderFile(filename));
	return shader;
}

GLenum Shader::type() const
{
	return _type;
}

void Shader::setSource(ShaderSource* source)
{
	if (_source) _source->deregisterListener(this);
	_source = source;
	if (_source) _source->registerListener(this);

	updateSource();
}

void Shader::setSource(const std::string& source)
{
	setSource(new ShaderCode(source));
}

void Shader::notifyChanged()
{
	updateSource();
}

void Shader::updateSource()
{
	std::string backup = _internalSource;

	if (_source)
	{
		basicSetSource(_source->source());
	}

	compile();

	if (!isCompiled())
	{
		basicSetSource(backup);

		compile();
	}
}

void Shader::basicSetSource(const std::string& source)
{
	_internalSource = source;
	const char* sourcePointer = source.c_str();
	glShaderSource(m_id, 1, &sourcePointer, 0);
}

void Shader::compile()
{
	glCompileShader(m_id);
	_compiled = checkCompileStatus();

	if (_compiled)
	{
		changed();
	}
}

bool Shader::isCompiled() const
{
	return _compiled;
}

std::string Shader::infoLog() const
{
	GLsizei length;
	glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);

	std::vector<char> log(length);

	glGetShaderInfoLog(m_id, length, &length, log.data());

	return std::string(log.data(), length);
}

bool Shader::checkCompileStatus()
{
	GLint status = 0;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);

	bool compiled = (status == GL_TRUE);

	if (!compiled)
	{
		error()
			<< "Compiler error:" << std::endl
			<< "Type " << typeString() << std::endl
			<< infoLog();
	}

	return compiled;
}

std::string Shader::typeString()
{
	switch (_type)
	{
		case GL_GEOMETRY_SHADER:
			return "GL_GEOMETRY_SHADER";
		case GL_FRAGMENT_SHADER:
			return "GL_FRAGMENT_SHADER";
		case GL_VERTEX_SHADER:
			return "GL_VERTEX_SHADER";
		case GL_TESS_EVALUATION_SHADER:
			return "GL_TESS_EVALUATION_SHADER";
		case GL_TESS_CONTROL_SHADER:
			return "GL_TESS_CONTROL_SHADER";
		#ifdef GL_COMPUTE_SHADER
		case GL_COMPUTE_SHADER:
			return "GL_COMPUTE_SHADER";
		#endif
		default:
			return "unknown";
	}
}

} // namespace glow
