#include <glow/ShaderCompiler.h>

#include <GL/glew.h>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/Shader.h>
#include <glow/StringSource.h>

namespace glow {

ShaderCompiler::ShaderCompiler(Shader* shader)
: m_shader(shader)
{
}

ShaderCompiler::~ShaderCompiler()
{
}

bool ShaderCompiler::compile(Shader* shader)
{
    return ShaderCompiler(shader).compile();
}

bool ShaderCompiler::compile()
{
    std::string source = m_shader->source()->string();
    const char * sourcePointer = source.c_str();

    glShaderSource(m_shader->id(), 1, &sourcePointer, 0);
    CheckGLError();

    if (glCompileShaderIncludeARB)
    {
        glCompileShaderIncludeARB(m_shader->id(), 0, nullptr, nullptr);
        CheckGLError();
    }
    else
    {
        glCompileShader(m_shader->id());
        CheckGLError();
    }

    bool compiled = checkCompileStatus();

    m_shader->m_compiled = compiled;

    if (compiled)
        m_shader->changed();

    return compiled;
}

bool ShaderCompiler::checkCompileStatus()
{
    GLint status = 0;

    glGetShaderiv(m_shader->id(), GL_COMPILE_STATUS, &status);
    CheckGLError();

    if (GL_FALSE == status)
    {
        critical()
            << "Compiler error:" << std::endl
            << m_shader->shaderString() << std::endl
            << m_shader->infoLog();

        return false;
    }

    return true;
}

} // namespace glow
