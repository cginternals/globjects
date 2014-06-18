#include <glow/Shader.h>

#include <vector>
#include <sstream>

#include <glbinding/constants.h>
#include <glbinding/functions.h>

#include <glowbase/ref_ptr.h>
#include <glowbase/Version.h>
#include <glowbase/AbstractStringSource.h>
#include <glowbase/StaticStringSource.h>
#include <glowbase/File.h>

#include <glow/Program.h>
#include <glow/logging.h>
#include <glow/ObjectVisitor.h>
#include <glow/glow.h>

#include "IncludeProcessor.h"

namespace
{

std::vector<const char*> collectCStrings(const std::vector<std::string> & strings)
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


Shader::Shader(const gl::GLenum type)
: Object(create(type))
, m_type(type)
, m_compiled(false)
, m_compilationFailed(false)
{
}


Shader::Shader(const gl::GLenum type, AbstractStringSource * source)
: Shader(type)
{
    setSource(source);
}

Shader::Shader(const gl::GLenum type, AbstractStringSource * source, const std::vector<std::string> & includePaths)
: Shader(type)
{
    setIncludePaths(includePaths);
    setSource(source);
}

Shader * Shader::fromString(const gl::GLenum type, const std::string & sourceString)
{
    return new Shader(type, new StaticStringSource(sourceString));
}

Shader * Shader::fromFile(const gl::GLenum type, const std::string & filename)
{
    return new Shader(type, new File(filename));
}

Shader::~Shader()
{
	if (m_source)
	{
		m_source->deregisterListener(this);
	}

	if (ownsGLObject())
	{
		gl::glDeleteShader(m_id);
	}
}

gl::GLuint Shader::create(gl::GLenum type)
{
    gl::GLuint result = gl::glCreateShader(type);

	return result;
}

void Shader::accept(ObjectVisitor& visitor)
{
	visitor.visitShader(this);
}

gl::GLenum Shader::type() const
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

void Shader::notifyChanged(const Changeable *)
{
	updateSource();
}

void Shader::updateSource()
{
    std::vector<std::string> sources;

    if (m_source)
    {
        if (hasExtension(gl::Extension::ARB_shading_language_include) && !forceFallbackIncludeProcessor)
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

    gl::glShaderSource(m_id, static_cast<gl::GLint>(cStrings.size()), cStrings.data(), nullptr);

    invalidate();
}

bool Shader::compile() const
{
    if (m_compilationFailed)
        return false;

    if (hasExtension(gl::Extension::ARB_shading_language_include) && !forceFallbackIncludeProcessor)
    {
        std::vector<const char*> cStrings = collectCStrings(m_includePaths);
        gl::glCompileShaderIncludeARB(m_id, static_cast<gl::GLint>(cStrings.size()), cStrings.data(), nullptr);
    }
    else
    {
        gl::glCompileShader(m_id);
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

gl::GLint Shader::get(gl::GLenum pname) const
{
    gl::GLint value = 0;
    gl::glGetShaderiv(m_id, pname, &value);

    return value;
}

std::string Shader::getSource() const
{
    gl::GLint sourceLength = get(gl::GL_SHADER_SOURCE_LENGTH);
    std::vector<char> source(sourceLength);

    gl::glGetShaderSource(m_id, sourceLength, nullptr, source.data());

    return std::string(source.data(), sourceLength);
}

bool Shader::checkCompileStatus() const
{
    gl::GLint status = get(gl::GL_COMPILE_STATUS);

    if (status == gl::GL_FALSE)
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
    gl::GLsizei length = get(gl::GL_INFO_LOG_LENGTH);
	std::vector<char> log(length);

	gl::glGetShaderInfoLog(m_id, length, &length, log.data());

	return std::string(log.data(), length);
}

std::string Shader::shaderString() const
{
	std::stringstream ss;

    ss << "Shader(" << typeString(m_type);

    std::string shortInfo = m_source->shortInfo();
    if (shortInfo.size() > 0)
        ss << ", " << shortInfo;

	ss << ")";

	return ss.str();
}

std::string Shader::typeString() const
{
    return typeString(m_type);
}

std::string Shader::typeString(gl::GLenum type)
{
    switch (type)
	{
    case gl::GL_GEOMETRY_SHADER:
        return "gl::GL_GEOMETRY_SHADER";
    case gl::GL_FRAGMENT_SHADER:
        return "gl::GL_FRAGMENT_SHADER";
    case gl::GL_VERTEX_SHADER:
        return "gl::GL_VERTEX_SHADER";
    case gl::GL_TESS_EVALUATION_SHADER:
        return "gl::GL_TESS_EVALUATION_SHADER";
    case gl::GL_TESS_CONTROL_SHADER:
        return "gl::GL_TESS_CONTROL_SHADER";
    case gl::GL_COMPUTE_SHADER:
        return "gl::GL_COMPUTE_SHADER";
	default:
		return "Unknown Shader Type";
	}
}

} // namespace glow
