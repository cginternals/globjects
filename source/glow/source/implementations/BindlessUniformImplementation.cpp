#include "BindlessUniformImplementation.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/functions.h>

#include <glow/Program.h>

namespace glow
{

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const float & value) const
{
    gl::ProgramUniform1f(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const int & value) const
{
    gl::ProgramUniform1i(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const unsigned int & value) const
{
    gl::ProgramUniform1ui(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const bool & value) const
{
    gl::ProgramUniform1i(program->id(), location, value ? 1 : 0);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec2 & value) const
{
    gl::ProgramUniform2fv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec3 & value) const
{
    gl::ProgramUniform3fv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::vec4 & value) const
{
    gl::ProgramUniform4fv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec2 & value) const
{
    gl::ProgramUniform2iv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec3 & value) const
{
    gl::ProgramUniform3iv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::ivec4 & value) const
{
    gl::ProgramUniform4iv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec2 & value) const
{
    gl::ProgramUniform2uiv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec3 & value) const
{
    gl::ProgramUniform3uiv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::uvec4 & value) const
{
    gl::ProgramUniform4uiv(program->id(), location, 1, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2 & value) const
{
    gl::ProgramUniformMatrix2fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3 & value) const
{
    gl::ProgramUniformMatrix3fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4 & value) const
{
    gl::ProgramUniformMatrix4fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2x3 & value) const
{
    gl::ProgramUniformMatrix2x3fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3x2 & value) const
{
    gl::ProgramUniformMatrix3x2fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat2x4 & value) const
{
    gl::ProgramUniformMatrix2x4fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4x2 & value) const
{
    gl::ProgramUniformMatrix4x2fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat3x4 & value) const
{
    gl::ProgramUniformMatrix3x4fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const glm::mat4x3 & value) const
{
    gl::ProgramUniformMatrix4x3fv(program->id(), location, 1, gl::FALSE_, glm::value_ptr(value));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const TextureHandle & value) const
{
    gl::ProgramUniformHandleui64ARB(program->id(), location, value);
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<float> & value) const
{
    gl::ProgramUniform1fv(program->id(), location, static_cast<gl::GLint>(value.size()), value.data());
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<int> & value) const
{
    gl::ProgramUniform1iv(program->id(), location, static_cast<gl::GLint>(value.size()), value.data());
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<unsigned int> & value) const
{
    gl::ProgramUniform1uiv(program->id(), location, static_cast<gl::GLint>(value.size()), value.data());
}

void BindlessUniformImplementation::set(const Program * program, gl::GLint location, const std::vector<bool> & value) const
{
    std::vector<int> values(value.size());
    for (unsigned i = 0; i < values.size(); ++i)
    {
        values[i] = value[i] ? 1 : 0;
    }

    gl::ProgramUniform1iv(program->id(), location, static_cast<gl::GLint>(values.size()), values.data());
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec2> & value) const
{
    gl::ProgramUniform2fv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec3> & value) const
{
    gl::ProgramUniform3fv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::vec4> & value) const
{
    gl::ProgramUniform4fv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec2> & value) const
{
    gl::ProgramUniform2iv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec3> & value) const
{
    gl::ProgramUniform3iv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::ivec4> & value) const
{
    gl::ProgramUniform4iv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec2> & value) const
{
    gl::ProgramUniform2uiv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec3> & value) const
{
    gl::ProgramUniform3uiv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::uvec4> & value) const
{
    gl::ProgramUniform4uiv(program->id(), location, static_cast<gl::GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2> & value) const
{
    gl::ProgramUniformMatrix2fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3> & value) const
{
    gl::ProgramUniformMatrix3fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4> & value) const
{
    gl::ProgramUniformMatrix4fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2x3> & value) const
{
    gl::ProgramUniformMatrix2x3fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3x2> & value) const
{
    gl::ProgramUniformMatrix3x2fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat2x4> & value) const
{
    gl::ProgramUniformMatrix2x4fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4x2> & value) const
{
    gl::ProgramUniformMatrix4x2fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat3x4> & value) const
{
    gl::ProgramUniformMatrix3x4fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<glm::mat4x3> & value) const
{
    gl::ProgramUniformMatrix4x3fv(program->id(), location, static_cast<gl::GLint>(value.size()), gl::FALSE_, reinterpret_cast<const float*>(value.data()));
}

void BindlessUniformImplementation::set(const Program * program, const gl::GLint location, const std::vector<TextureHandle> & value) const
{
    const TextureHandle * handle = value.data();
    gl::ProgramUniformHandleui64vARB(program->id(), location, static_cast<gl::GLint>(value.size()), handle);
}

} // namespace glow
