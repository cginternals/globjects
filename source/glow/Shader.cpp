
#include <vector>
#include <sstream>

#include <glow/Program.h>
#include <glow/logging.h>
#include <glow/ShaderFile.h>
#include <glow/ShaderSource.h>
#include <glow/ShaderCode.h>
#include <glow/Error.h>
#include <glow/ObjectVisitor.h>

#include <glow/Shader.h>

namespace glow
{

Shader::Shader(
    const GLenum type
,   ShaderSource * source)
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

Shader * Shader::fromFile(
    const GLenum type
,   const std::string & filePath)
{
    ShaderFile * source = new ShaderFile(filePath);
    return new Shader(type, source);
}

Shader * Shader::fromString(
    const GLenum type
    , const std::string & sourceString)
{
    ShaderCode * source = new ShaderCode(sourceString);
    return new Shader(type, source);
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

void Shader::setSource(ShaderSource * source)
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
	setSource(new ShaderCode(source));
}

const ShaderSource* Shader::source() const
{
	return m_source;
}

void Shader::notifyChanged()
{
	updateSource();
}

void Shader::updateSource()
{
	if (m_source)
        setSource(*this, m_source->source());

	if(!compile())
	{
        setSource(*this, m_currentSource);
        compile();
    }
    else
	{
        m_currentSource = m_source->source();
	}
}

void Shader::setSource(
    const Shader & shader
,   const std::string & source)
{
	const char * sourcePointer = source.c_str();

	glShaderSource(shader.m_id, 1, &sourcePointer, 0);
	CheckGLError();
}

bool Shader::compile()
{
	glCompileShader(m_id);
	CheckGLError();

	m_compiled = checkCompileStatus();

	if (m_compiled)
		changed();

    return isCompiled();
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

bool Shader::checkCompileStatus()
{
	GLint status = 0;

	glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
	CheckGLError();

	if (GL_FALSE == status)
	{
		critical()
			<< "Compiler error:" << std::endl
			<< shaderString() << std::endl
			<< infoLog();

        return false;
	}

	return true;
}

std::string Shader::shaderString() const
{
	std::stringstream ss;

	ss << "Shader(" << typeString();

	if (m_source && m_source->isFile())
		ss << ", " << dynamic_cast<const ShaderFile*>(*m_source)->filePath();

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
