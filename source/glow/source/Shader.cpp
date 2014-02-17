#include <glow/Shader.h>

#include <vector>
#include <sstream>

#include <glow/Program.h>
#include <glow/logging.h>
#include <glow/AbstractStringSource.h>
#include <glow/StaticStringSource.h>
#include <glow/Error.h>
#include <glow/ObjectVisitor.h>
#include <glow/Version.h>
#include <glow/Extension.h>
#include <glow/ref_ptr.h>

#include "IncludeProcessor.h"

namespace
{

std::vector<const char*> collectCStrings(std::vector<std::string> & strings)
{
    std::vector<const char*> cStrings;

    for (const std::string & str : strings)
    {
        cStrings.push_back(str.c_str());
    }

    return cStrings;
}

}

namespace glow
{

bool Shader::forceFallbackIncludeProcessor = false;


Shader::Shader(const GLenum type)
: Object(create(type))
, m_type(type)
, m_compiled(false)
, m_compilationFailed(false)
{
}


Shader::Shader(const GLenum type, AbstractStringSource * source)
: Shader(type)
{
    setSource(source);
}

Shader::Shader(const GLenum type, AbstractStringSource * source, const std::vector<std::string> & includePaths)
: Shader(type)
{
    setIncludePaths(includePaths);
    setSource(source);
}

Shader * Shader::fromString(const GLenum type, const std::string & sourceString)
{
    return new Shader(type, new StaticStringSource(sourceString));
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

void Shader::setSource(AbstractStringSource * source)
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
    setSource(new StaticStringSource(source));
}

const AbstractStringSource* Shader::source() const
{
	return m_source;
}

void Shader::notifyChanged(Changeable *)
{
	updateSource();
}

void Shader::updateSource()
{
    std::vector<std::string> sources;

    if (m_source)
    {
        if (glow::hasExtension(GLOW_ARB_shading_language_include) && !forceFallbackIncludeProcessor)
        {
            sources = m_source->strings();
        }
        else
        {
            ref_ptr<AbstractStringSource> resolvedSource = IncludeProcessor::resolveIncludes(m_source, m_includePaths);

            sources = resolvedSource->strings();
        }
    }

    std::vector<const char*> cStrings = collectCStrings(sources);

    glShaderSource(m_id, static_cast<GLint>(cStrings.size()), cStrings.data(), nullptr);
    CheckGLError();

    invalidate();
}

bool Shader::compile()
{
    if (m_compilationFailed)
        return false;

    if (glow::hasExtension(GLOW_ARB_shading_language_include) && !forceFallbackIncludeProcessor)
    {
        std::vector<const char*> cStrings = collectCStrings(m_includePaths);
        glCompileShaderIncludeARB(m_id, static_cast<GLint>(cStrings.size()), cStrings.data(), nullptr);
        CheckGLError();
    }
    else
    {
        glCompileShader(m_id);
        CheckGLError();
    }

    m_compiled = checkCompileStatus();

    m_compilationFailed = !m_compiled;

    changed();

    return m_compiled;
}

bool Shader::isCompiled() const
{
	return m_compiled;
}

void Shader::invalidate()
{
    m_compiled = false;
    m_compilationFailed = false;
    changed();
}

void Shader::setIncludePaths(const std::vector<std::string> & includePaths)
{
    m_includePaths = includePaths;

    invalidate();
}

GLint Shader::get(GLenum pname) const
{
    GLint value = 0;
    glGetShaderiv(m_id, pname, &value);
    CheckGLError();

    return value;
}

std::string Shader::getSource() const
{
    GLint sourceLength = get(GL_SHADER_SOURCE_LENGTH);
    std::vector<char> source(sourceLength);

    glGetShaderSource(m_id, sourceLength, nullptr, source.data());
    CheckGLError();

    return std::string(source.data(), sourceLength);
}

bool Shader::checkCompileStatus() const
{
    GLint status = get(GL_COMPILE_STATUS);

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

std::string Shader::infoLog() const
{
    GLsizei length = get(GL_INFO_LOG_LENGTH);
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
