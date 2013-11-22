#include <vector>
#include <sstream>

#include <glow/Program.h>
#include <glow/logging.h>
#include <glow/StringSource.h>
#include <glow/String.h>
#include <glow/Error.h>
#include <glow/ObjectVisitor.h>

#include <glow/Shader.h>
#include <glow/ShaderCompiler.h>

namespace glow
{

Shader::Shader(
    const GLenum type
,   StringSource * source)
:   Object(create(type))
,   m_type(type)
,   m_source(nullptr)
,   m_compiled(false)
{
	if (source)
		setSource(source);
}

Shader::Shader(const GLenum type)
:   Shader(type, nullptr)
{
}

Shader * Shader::fromString(
    const GLenum type
    , const std::string & sourceString)
{
    return new Shader(type, new String(sourceString));
}

Shader::~Shader()
{
	if (m_source)
	{
		m_source->deregisterListener(this);
	}

	if (ownsGLObject())
	{
		glDeleteShader(m_id);
		CheckGLError();
	}
}

GLuint Shader::create(GLenum type)
{
	GLuint result = glCreateShader(type);
	CheckGLError();
	return result;
}

void Shader::accept(ObjectVisitor& visitor)
{
	visitor.visitShader(this);
}

GLenum Shader::type() const
{
	return m_type;
}

void Shader::setSource(StringSource * source)
{
    if (source == m_source)
        return;

	if (m_source)
		m_source->deregisterListener(this);

	m_source = source;

	if (m_source)
		m_source->registerListener(this);

	updateSource();
}

void Shader::setSource(const std::string & source)
{
    setSource(new String(source));
}

const StringSource* Shader::source() const
{
	return m_source;
}

void Shader::notifyChanged()
{
	updateSource();
}

void Shader::updateSource()
{
    std::string backupSource = m_currentSource;

    m_currentSource = m_source->string();

    if (!ShaderCompiler::compile(this))
    {
        m_currentSource = backupSource;

        ShaderCompiler::compile(this);
    }
}

bool Shader::isCompiled() const
{
	return m_compiled;
}

std::string Shader::infoLog() const
{
	GLsizei length;

	glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
	CheckGLError();

	std::vector<char> log(length);

	glGetShaderInfoLog(m_id, length, &length, log.data());
	CheckGLError();

	return std::string(log.data(), length);
}

std::string Shader::shaderString() const
{
	std::stringstream ss;

	ss << "Shader(" << typeString();

    std::string shortInfo = m_source->shortInfo();
    if (shortInfo.size() > 0)
        ss << ", " << shortInfo;

	ss << ")";

	return ss.str();
}

std::string Shader::typeString() const
{
	switch (m_type)
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
	case GL_COMPUTE_SHADER:
		return "GL_COMPUTE_SHADER";
	default:
		return "Unknown Shader Type";
	}
}

} // namespace glow
