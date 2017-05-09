
#include <globjects/Shader.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>

#include <globjects/Program.h>
#include <globjects/ObjectVisitor.h>

#include <globjects/Resource.h>

#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractShadingLanguageIncludeImplementation.h"

using namespace gl;


namespace
{


const globjects::AbstractShadingLanguageIncludeImplementation & shadingLanguageIncludeImplementation()
{
    return globjects::ImplementationRegistry::current().shadingLanguageIncludeImplementation();
}


} // namespace


namespace globjects
{


void Shader::hintIncludeImplementation(const IncludeImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}

std::map<std::string, std::string> Shader::s_globalReplacements;


Shader::Shader(const GLenum type)
: Object(std::unique_ptr<IDResource>(new ShaderResource(type)))
, m_type(type)
, m_source(nullptr)
, m_compiled(false)
, m_compilationFailed(false)
{
}

Shader::Shader(const GLenum type, AbstractStringSource * source, const IncludePaths & includePaths)
: Shader(type)
{
    setIncludePaths(includePaths);
    setSource(source);
}

std::unique_ptr<StaticStringSource> Shader::sourceFromString(const std::string & sourceString)
{
    return StaticStringSource::create(sourceString);
}

std::unique_ptr<File> Shader::sourceFromFile(const std::string & filename)
{
    return File::create(filename, false);
}

std::unique_ptr<AbstractStringSource> Shader::applyGlobalReplacements(AbstractStringSource * source)
{
    auto sourceTemplate = StringTemplate::create(source);

    for (const auto & pair : s_globalReplacements)
    {
        sourceTemplate->replace(pair.first, pair.second);
    }

    return std::unique_ptr<AbstractStringSource>(sourceTemplate.release());
}

Shader::~Shader()
{
    while (!m_programs.empty())
    {
        (*m_programs.begin())->detach(this);
    }
}

void Shader::globalReplace(const std::string & search, const std::string & replacement)
{
    s_globalReplacements[search] = replacement;
}

void Shader::globalReplace(const std::string & search, const int i)
{
    globalReplace(search, std::to_string(i));
}

void Shader::clearGlobalReplacements()
{
    s_globalReplacements.clear();
}

void Shader::accept(ObjectVisitor & visitor)
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
    shadingLanguageIncludeImplementation().updateSources(this);

    invalidate();
}

bool Shader::compile() const
{
    if (m_compilationFailed)
        return false;

    shadingLanguageIncludeImplementation().compile(this);

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

const Shader::IncludePaths & Shader::includePaths() const
{
    return m_includePaths;
}

void Shader::setIncludePaths(const std::vector<std::string> & includePaths)
{
    m_includePaths = includePaths;

    invalidate();
}

GLint Shader::get(GLenum pname) const
{
    GLint value = 0;
    glGetShaderiv(id(), pname, &value);

    return value;
}

std::string Shader::getSource() const
{
    GLint sourceLength = get(GL_SHADER_SOURCE_LENGTH);
    std::vector<char> source(sourceLength);

    glGetShaderSource(id(), sourceLength, nullptr, source.data());

    return std::string(source.data(), sourceLength);
}

bool Shader::checkCompileStatus() const
{
    GLboolean status = static_cast<GLboolean>(get(GL_COMPILE_STATUS));

    if (status == GL_FALSE)
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

    glGetShaderInfoLog(id(), length, &length, log.data());

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

std::string Shader::typeString(GLenum type)
{
    switch (type)
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

GLenum Shader::objectType() const
{
    return GL_SHADER;
}


} // namespace globjects
