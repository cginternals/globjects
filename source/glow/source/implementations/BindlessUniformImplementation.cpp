#include "BindlessUniformImplementation.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/functions.h>

#include <glow/Program.h>

namespace glow
{

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const float & value) const
{
    gl::glProgramUniform1f(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const int & value) const
{
    gl::glProgramUniform1i(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const unsigned int & value) const
{
    gl::glProgramUniform1ui(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const bool & value) const
{
    gl::glProgramUniform1i(program->id(), location, value ? 1 : 0);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec2 & value) const
{
    gl::glProgramUniform2fv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec3 & value) const
{
    gl::glProgramUniform3fv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec4 & value) const
{
    gl::glProgramUniform4fv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec2 & value) const
{
    gl::glProgramUniform2iv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec3 & value) const
{
    gl::glProgramUniform3iv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec4 & value) const
{
    gl::glProgramUniform4iv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec2 & value) const
{
    gl::glProgramUniform2uiv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec3 & value) const
{
    gl::glProgramUniform3uiv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec4 & value) const
{
    gl::glProgramUniform4uiv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2 & value) const
{
    gl::glProgramUniformMatrix2fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3 & value) const
{
    gl::glProgramUniformMatrix3fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4 & value) const
{
    gl::glProgramUniformMatrix4fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2x3 & value) const
{
    gl::glProgramUniformMatrix2x3fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3x2 & value) const
{
    gl::glProgramUniformMatrix3x2fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2x4 & value) const
{
    gl::glProgramUniformMatrix2x4fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4x2 & value) const
{
    gl::glProgramUniformMatrix4x2fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3x4 & value) const
{
    gl::glProgramUniformMatrix3x4fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4x3 & value) const
{
    gl::glProgramUniformMatrix4x3fv(program->id(), location, 1, gl::GL_FALSE, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const TextureHandle & value) const
{
    gl::glProgramUniformHandleui64ARB(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<float> & value) const
{
    gl::glProgramUniform1fv(program->id(), location, static_cast<gl::GLint>(value.size()), value.data());
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<int> & value) const
{
    gl::glProgramUniform1iv(program->id(), location, static_cast<gl::GLint>(value.size()), value.data());
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<unsigned int> & value) const
{
    gl::glProgramUniform1uiv(program->id(), location, static_cast<gl::GLint>(value.size()), value.data());
}

void BindlessUniformImplementation::set(const Program * program, gl::GLint location, const std::vector<bool> & value) const
{
    std::vector<int> values(value.size());
    for (unsigned i = 0; i < values.size(); ++i)
    {
        values[i] = value[i] ? 1 : 0;
    }

    gl::glProgramUniform1iv(program->id(), location, static_cast<gl::GLint>(values.size()), values.data());
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec2> & value) const
{
    gl::glProgramUniform2fv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec3> & value) const
{
    gl::glProgramUniform3fv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec4> & value) const
{
    gl::glProgramUniform4fv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec2> & value) const
{
    gl::glProgramUniform2iv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec3> & value) const
{
    gl::glProgramUniform3iv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec4> & value) const
{
    gl::glProgramUniform4iv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec2> & value) const
{
    gl::glProgramUniform2uiv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec3> & value) const
{
    gl::glProgramUniform3uiv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec4> & value) const
{
    gl::glProgramUniform4uiv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2> & value) const
{
    gl::glProgramUniformMatrix2fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3> & value) const
{
    gl::glProgramUniformMatrix3fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4> & value) const
{
    gl::glProgramUniformMatrix4fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2x3> & value) const
{
    gl::glProgramUniformMatrix2x3fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3x2> & value) const
{
    gl::glProgramUniformMatrix3x2fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2x4> & value) const
{
    gl::glProgramUniformMatrix2x4fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4x2> & value) const
{
    gl::glProgramUniformMatrix4x2fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3x4> & value) const
{
    gl::glProgramUniformMatrix3x4fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4x3> & value) const
{
    gl::glProgramUniformMatrix4x3fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<TextureHandle> & value) const
{
    const TextureHandle * handle = value.data();
    gl::glProgramUniformHandleui64vARB(program->id(), location, static_cast<gl::GLint>(value.size()), handle);
}

} // namespace glow
