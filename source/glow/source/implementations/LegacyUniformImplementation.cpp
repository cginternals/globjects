#include "LegacyUniformImplementation.h"

#include <glow/Error.h>

namespace glow
{

void LegacyUniformImplementation::set(const Program * program, const GLint location, const float & value) const
{
    program->use();
    glUniform1f(location, value);
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const int & value) const
{
    program->use();
    glUniform1i(location, value);
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const unsigned int & value) const
{
    program->use();
    glUniform1ui(location, value);
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const bool & value) const
{
    program->use();
    glUniform1i(location, value ? 1 : 0);
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::vec2 & value) const
{
    program->use();
    glUniform2fv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::vec3 & value) const
{
    program->use();
    glUniform3fv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::vec4 & value) const
{
    program->use();
    glUniform4fv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::ivec2 & value) const
{
    program->use();
    glUniform2iv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::ivec3 & value) const
{
    program->use();
    glUniform3iv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::ivec4 & value) const
{
    program->use();
    glUniform4iv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::uvec2 & value) const
{
    program->use();
    glUniform2uiv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::uvec3 & value) const
{
    program->use();
    glUniform3uiv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::uvec4 & value) const
{
    program->use();
    glUniform4uiv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat2 & value) const
{
    program->use();
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat3 & value) const
{
    program->use();
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat4 & value) const
{
    program->use();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat2x3 & value) const
{
    program->use();
    glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat3x2 & value) const
{
    program->use();
    glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat2x4 & value) const
{
    program->use();
    glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat4x2 & value) const
{
    program->use();
    glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat3x4 & value) const
{
    program->use();
    glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const glm::mat4x3 & value) const
{
    program->use();
    glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const TextureHandle & value) const
{
    program->use();
    glUniformHandleui64ARB(location, value);
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<float> & value) const
{
    program->use();
    glUniform1fv(location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<int> & value) const
{
    program->use();
    glUniform1iv(location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<unsigned int> & value) const
{
    program->use();
    glUniform1uiv(location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, GLint location, const std::vector<bool> & value) const
{
    std::vector<int> values(value.size());
    for (unsigned i = 0; i < values.size(); ++i)
    {
        values[i] = value[i] ? 1 : 0;
    }

    program->use();
    glUniform1iv(location, static_cast<GLint>(values.size()), values.data());
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::vec2> & value) const
{
    program->use();
    glUniform2fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::vec3> & value) const
{
    program->use();
    glUniform3fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::vec4> & value) const
{
    program->use();
    glUniform4fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::ivec2> & value) const
{
    program->use();
    glUniform2iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::ivec3> & value) const
{
    program->use();
    glUniform3iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::ivec4> & value) const
{
    program->use();
    glUniform4iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::uvec2> & value) const
{
    program->use();
    glUniform2uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::uvec3> & value) const
{
    program->use();
    glUniform3uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::uvec4> & value) const
{
    program->use();
    glUniform4uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat2> & value) const
{
    program->use();
    glUniformMatrix2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat3> & value) const
{
    program->use();
    glUniformMatrix3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat4> & value) const
{
    program->use();
    glUniformMatrix4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat2x3> & value) const
{
    program->use();
    glUniformMatrix2x3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat3x2> & value) const
{
    program->use();
    glUniformMatrix3x2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat2x4> & value) const
{
    program->use();
    glUniformMatrix2x4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat4x2> & value) const
{
    program->use();
    glUniformMatrix4x2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat3x4> & value) const
{
    program->use();
    glUniformMatrix3x4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat4x3> & value) const
{
    program->use();
    glUniformMatrix4x3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void LegacyUniformImplementation::set(const Program * program, const GLint location, const std::vector<TextureHandle> & value) const
{
    program->use();
    glUniformHandleui64vARB(location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

} // namespace glow
