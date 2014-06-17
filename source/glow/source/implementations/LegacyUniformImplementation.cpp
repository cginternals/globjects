#include "LegacyUniformImplementation.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/functions.h>

#include <glow/Program.h>

namespace glow
{

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const float & value) const
{
    program->use();
    gl::glUniform1f(location, value);
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const int & value) const
{
    program->use();
    gl::glUniform1i(location, value);
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const unsigned int & value) const
{
    program->use();
    gl::glUniform1ui(location, value);
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const bool & value) const
{
    program->use();
    gl::glUniform1i(location, value ? 1 : 0);
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec2 & value) const
{
    program->use();
    gl::glUniform2fv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec3 & value) const
{
    program->use();
    gl::glUniform3fv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec4 & value) const
{
    program->use();
    gl::glUniform4fv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec2 & value) const
{
    program->use();
    gl::glUniform2iv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec3 & value) const
{
    program->use();
    gl::glUniform3iv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec4 & value) const
{
    program->use();
    gl::glUniform4iv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec2 & value) const
{
    program->use();
    gl::glUniform2uiv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec3 & value) const
{
    program->use();
    gl::glUniform3uiv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec4 & value) const
{
    program->use();
    gl::glUniform4uiv(location, 1, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2 & value) const
{
    program->use();
    gl::glUniformMatrix2fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3 & value) const
{
    program->use();
    gl::glUniformMatrix3fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4 & value) const
{
    program->use();
    gl::glUniformMatrix4fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2x3 & value) const
{
    program->use();
    gl::glUniformMatrix2x3fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3x2 & value) const
{
    program->use();
    gl::glUniformMatrix3x2fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2x4 & value) const
{
    program->use();
    gl::glUniformMatrix2x4fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4x2 & value) const
{
    program->use();
    gl::glUniformMatrix4x2fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3x4 & value) const
{
    program->use();
    gl::glUniformMatrix3x4fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4x3 & value) const
{
    program->use();
    gl::glUniformMatrix4x3fv(location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const TextureHandle & value) const
{
    program->use();
    gl::glUniformHandleui64ARB(location, value);
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<float> & value) const
{
    program->use();
    gl::glUniform1fv(location, static_cast<gl::GLint>(value.size()), value.data());
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<int> & value) const
{
    program->use();
    gl::glUniform1iv(location, static_cast<gl::GLint>(value.size()), value.data());
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<unsigned int> & value) const
{
    program->use();
    gl::glUniform1uiv(location, static_cast<gl::GLint>(value.size()), value.data());
}

void LegacyUniformImplementation::set(const Program * program, gl::GLint location, const std::vector<bool> & value) const
{
    std::vector<int> values(value.size());
    for (unsigned i = 0; i < values.size(); ++i)
    {
        values[i] = value[i] ? 1 : 0;
    }

    program->use();
    gl::glUniform1iv(location, static_cast<gl::GLint>(values.size()), values.data());
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec2> & value) const
{
    program->use();
    gl::glUniform2fv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec3> & value) const
{
    program->use();
    gl::glUniform3fv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec4> & value) const
{
    program->use();
    gl::glUniform4fv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec2> & value) const
{
    program->use();
    gl::glUniform2iv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec3> & value) const
{
    program->use();
    gl::glUniform3iv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec4> & value) const
{
    program->use();
    gl::glUniform4iv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec2> & value) const
{
    program->use();
    gl::glUniform2uiv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec3> & value) const
{
    program->use();
    gl::glUniform3uiv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec4> & value) const
{
    program->use();
    gl::glUniform4uiv(location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2> & value) const
{
    program->use();
    gl::glUniformMatrix2fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3> & value) const
{
    program->use();
    gl::glUniformMatrix3fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4> & value) const
{
    program->use();
    gl::glUniformMatrix4fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2x3> & value) const
{
    program->use();
    gl::glUniformMatrix2x3fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3x2> & value) const
{
    program->use();
    gl::glUniformMatrix3x2fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2x4> & value) const
{
    program->use();
    gl::glUniformMatrix2x4fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4x2> & value) const
{
    program->use();
    gl::glUniformMatrix4x2fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3x4> & value) const
{
    program->use();
    gl::glUniformMatrix3x4fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4x3> & value) const
{
    program->use();
    gl::glUniformMatrix4x3fv(location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void LegacyUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<TextureHandle> & value) const
{
    program->use();
    gl::glUniformHandleui64vARB(location, static_cast<gl::GLint>(value.size()), value.data());
}

} // namespace glow
