#include "BindlessUniformImplementation.h"

#include <glm/gtc/type_ptr.hpp>

#include <glow/Error.h>

namespace glow
{

void BindlessUniformImplementation::set(const Program * program, const GLint location, const float & value) const
{
    glProgramUniform1f(program->id(), location, value);
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const int & value) const
{
    glProgramUniform1i(program->id(), location, value);
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const unsigned int & value) const
{
    glProgramUniform1ui(program->id(), location, value);
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const bool & value) const
{
    glProgramUniform1i(program->id(), location, value ? 1 : 0);
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::vec2 & value) const
{
    glProgramUniform2fv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::vec3 & value) const
{
    glProgramUniform3fv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::vec4 & value) const
{
    glProgramUniform4fv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::ivec2 & value) const
{
    glProgramUniform2iv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::ivec3 & value) const
{
    glProgramUniform3iv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::ivec4 & value) const
{
    glProgramUniform4iv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::uvec2 & value) const
{
    glProgramUniform2uiv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::uvec3 & value) const
{
    glProgramUniform3uiv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::uvec4 & value) const
{
    glProgramUniform4uiv(program->id(), location, 1, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat2 & value) const
{
    glProgramUniformMatrix2fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat3 & value) const
{
    glProgramUniformMatrix3fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat4 & value) const
{
    glProgramUniformMatrix4fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat2x3 & value) const
{
    glProgramUniformMatrix2x3fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat3x2 & value) const
{
    glProgramUniformMatrix3x2fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat2x4 & value) const
{
    glProgramUniformMatrix2x4fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat4x2 & value) const
{
    glProgramUniformMatrix4x2fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat3x4 & value) const
{
    glProgramUniformMatrix3x4fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const glm::mat4x3 & value) const
{
    glProgramUniformMatrix4x3fv(program->id(), location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const TextureHandle & value) const
{
    glProgramUniformHandleui64ARB(program->id(), location, value);
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<float> & value) const
{
    glProgramUniform1fv(program->id(), location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<int> & value) const
{
    glProgramUniform1iv(program->id(), location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<unsigned int> & value) const
{
    glProgramUniform1uiv(program->id(), location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, GLint location, const std::vector<bool> & value) const
{
    std::vector<int> values(value.size());
    for (unsigned i = 0; i < values.size(); ++i)
    {
        values[i] = value[i] ? 1 : 0;
    }

    glProgramUniform1iv(program->id(), location, static_cast<GLint>(values.size()), values.data());
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::vec2> & value) const
{
    glProgramUniform2fv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::vec3> & value) const
{
    glProgramUniform3fv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::vec4> & value) const
{
    glProgramUniform4fv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::ivec2> & value) const
{
    glProgramUniform2iv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::ivec3> & value) const
{
    glProgramUniform3iv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::ivec4> & value) const
{
    glProgramUniform4iv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::uvec2> & value) const
{
    glProgramUniform2uiv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::uvec3> & value) const
{
    glProgramUniform3uiv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::uvec4> & value) const
{
    glProgramUniform4uiv(program->id(), location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat2> & value) const
{
    glProgramUniformMatrix2fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat3> & value) const
{
    glProgramUniformMatrix3fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat4> & value) const
{
    glProgramUniformMatrix4fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat2x3> & value) const
{
    glProgramUniformMatrix2x3fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat3x2> & value) const
{
    glProgramUniformMatrix3x2fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat2x4> & value) const
{
    glProgramUniformMatrix2x4fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat4x2> & value) const
{
    glProgramUniformMatrix4x2fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat3x4> & value) const
{
    glProgramUniformMatrix3x4fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<glm::mat4x3> & value) const
{
    glProgramUniformMatrix4x3fv(program->id(), location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void BindlessUniformImplementation::set(const Program * program, const GLint location, const std::vector<TextureHandle> & value) const
{
    const TextureHandle * handle = value.data();
    glProgramUniformHandleui64vARB(program->id(), location, static_cast<GLint>(value.size()), handle);
    CheckGLError();
}

} // namespace glow
