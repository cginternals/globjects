#include <glow/ProgramUniformSetter.h>

#include <glow/Error.h>

namespace glow
{

void ProgramUniformSetter::set(GLuint program, const GLint location, const float& value)
{
    glProgramUniform1f(program, location, value);
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const int & value)
{
    glProgramUniform1i(program, location, value);
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const unsigned int & value)
{
    glProgramUniform1ui(program, location, value);
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const bool & value)
{
    glProgramUniform1i(program, location, value ? 1 : 0);
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::vec2 & value)
{
    glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::vec3 & value)
{
    glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::vec4 & value)
{
    glProgramUniform4fv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::ivec2 & value)
{
    glProgramUniform2iv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::ivec3 & value)
{
    glProgramUniform3iv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::ivec4 & value)
{
    glProgramUniform4iv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::uvec2 & value)
{
    glProgramUniform2uiv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::uvec3 & value)
{
    glProgramUniform3uiv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::uvec4 & value)
{
    glProgramUniform4uiv(program, location, 1, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat2 & value)
{
    glProgramUniformMatrix2fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat3 & value)
{
    glProgramUniformMatrix3fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat4 & value)
{
    glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat2x3 & value)
{
    glProgramUniformMatrix2x3fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat3x2 & value)
{
    glProgramUniformMatrix3x2fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat2x4 & value)
{
    glProgramUniformMatrix2x4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat4x2 & value)
{
    glProgramUniformMatrix4x2fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat3x4 & value)
{
    glProgramUniformMatrix3x4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const glm::mat4x3 & value)
{
    glProgramUniformMatrix4x3fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const TextureHandle & value)
{
    glProgramUniformHandleui64ARB(program, location, value);
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<float> & value)
{
    glProgramUniform1fv(program, location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<int> & value)
{
    glProgramUniform1iv(program, location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<unsigned int> & value)
{
    glProgramUniform1uiv(program, location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::vec2> & value)
{
    glProgramUniform2fv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::vec3> & value)
{
    glProgramUniform3fv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::vec4> & value)
{
    glProgramUniform4fv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::ivec2> & value)
{
    glProgramUniform2iv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::ivec3> & value)
{
    glProgramUniform3iv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::ivec4> & value)
{
    glProgramUniform4iv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::uvec2> & value)
{
    glProgramUniform2uiv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::uvec3> & value)
{
    glProgramUniform3uiv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::uvec4> & value)
{
    glProgramUniform4uiv(program, location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat2> & value)
{
    glProgramUniformMatrix2fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat3> & value)
{
    glProgramUniformMatrix3fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat4> & value)
{
    glProgramUniformMatrix4fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat2x3> & value)
{
    glProgramUniformMatrix2x3fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat3x2> & value)
{
    glProgramUniformMatrix3x2fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat2x4> & value)
{
    glProgramUniformMatrix2x4fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat4x2> & value)
{
    glProgramUniformMatrix4x2fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat3x4> & value)
{
    glProgramUniformMatrix3x4fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<glm::mat4x3> & value)
{
    glProgramUniformMatrix4x3fv(program, location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
    CheckGLError();
}

void ProgramUniformSetter::set(GLuint program, const GLint location, const std::vector<TextureHandle> & value)
{
    const TextureHandle * handle = value.data();
    glProgramUniformHandleui64vARB(program, location, static_cast<GLint>(value.size()), handle);
    CheckGLError();
}

} // namespace glow
