
#include <algorithm>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>

#include <glow/program.h>

#include <glow/shader.h>
#include <glow/gpuquery.h>


namespace glow
{

Program::Program()
:   m_program(-1)
,   m_linked(false)
,   m_dirty(true)
{
    m_program = glCreateProgram();
    glError();
}

Program::~Program()
{
    if(isProgram())
    {
        while(m_shaders.cend() != m_shaders.cbegin())
            detach(*(m_shaders.begin()));

        glDeleteProgram(m_program);
        glError();

        m_program = 0;
    }
}

inline const bool Program::isProgram() const
{
    return m_program != -1;
}

const bool Program::isUsed() const
{
    GLint program(-1);
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    if(-1 == program)
        return false;

    return program == m_program;
}

const bool Program::use() const
{
    if(m_dirty)
        link();

    if(!m_linked)
        return false;

    if(isUsed())
        return true;

    glUseProgram(m_program);
    return !glError();
}

const bool Program::release() const
{
    assert(isUsed());

    glUseProgram(0);
    return !glError();
}

const bool Program::link() const
{
    if(!m_dirty)
        return isLinked();

    for(const Shader * shader : m_shaders)
        if(!shader->isCompiled())
            return false;

    m_dirty = false;

    glLinkProgram(m_program);
    glError();

    GLint status(GL_FALSE);
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    glError();

    m_linked = (GL_TRUE == status);
    m_log = "";

    // check for compile errors

    GLint maxLength(0);
    GLint logLength(0);

    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
    glError();

    if(!maxLength)
        return isLinked();

    GLchar *log = new GLchar[maxLength];
    glGetProgramInfoLog(m_program, maxLength, &logLength, log);
    glError();

    m_log = log;

    if(!m_log.isEmpty() && m_log != "No errors.\n")
        qWarning("%s", log);

    return isLinked();
}

const bool Program::attach(Shader * shader)
{
    if(!shader)
        return false;

    if(m_shaders.end() != m_shaders.find(shader))
        return true;

    m_shaders.insert(shader);
    shader->programs().insert(this);

    glAttachShader(m_program, shader->shader());
    m_dirty = true;

    return !glError();
}

const bool Program::detach(Shader * shader)
{
    if(!shader)
        return false;
    if(m_shaders.end() == m_shaders.find(shader))
        return true;

    glDetachShader(m_program, shader->shader());
    const bool result(glError());

    m_dirty = true;

    shader->programs().remove(this);
    m_shaders.remove(shader);

    if(shader->programs().empty())
        delete shader;

    return !result;
}

const Program::t_shaders & Program::shaders()
{
    return m_shaders;
}

const GLuint Program::program() const
{
    return m_program;
}

void Program::invalidate()
{
    m_dirty = true;
    link();
}

const bool Program::isLinked() const
{
    return m_linked;
}

const GLint Program::attributeLocation(const std::string & name) const
{
    if(m_dirty || !m_linked)
        link();

    const GLchar * chr(name.c_str());
    const GLint location = glGetAttribLocation(m_program, chr);
    glError();

    return location;
}

const GLint Program::uniformLocation(const std::string & name) const
{
    if(m_dirty || !m_linked)
        link();

    use();

    const GLchar * chr(name.c_str());
    const GLint location = glGetUniformLocation(m_program, chr);
    glError();

    return location;
}

//void Program::setUniform(
//    const QString & name
//,   const float value) const
//{
//    const GLint location(uniformLocation(name));
//
//    glUniform1f(location, value);
//    glError();
//}

} // namespace glow