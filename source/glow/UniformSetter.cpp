#include <glow/UniformSetter.h>

#include <glow/Error.h>

namespace glow
{

void UniformSetter::set(const GLint location, const float& value)
{
    glUniform1f(location, value);
    CheckGLError();
}
void UniformSetter::set(const GLint location, const int & value)
{
    glUniform1i(location, value);
    CheckGLError();
}
void UniformSetter::set(const GLint location, const unsigned int & value)
{
    glUniform1ui(location, value);
    CheckGLError();
}
void UniformSetter::set(const GLint location, const bool & value)
{
    glUniform1i(location, value ? 1 : 0);
    CheckGLError();
}

void UniformSetter::set(const GLint location, const glm::vec2 & value)
{
    glUniform2fv(location, 1, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::vec3 & value)
{
    glUniform3fv(location, 1, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::vec4 & value)
{
    glUniform4fv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const glm::ivec2 & value)
{
    glUniform2iv(location, 1, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::ivec3 & value)
{
    glUniform3iv(location, 1, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::ivec4 & value)
{
    glUniform4iv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const glm::uvec2 & value)
{
    glUniform2uiv(location, 1, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::uvec3 & value)
{
    glUniform3uiv(location, 1, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::uvec4 & value)
{
    glUniform4uiv(location, 1, glm::value_ptr(value));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const glm::mat2 & value)
{
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::mat3 & value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::mat4 & value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const glm::mat2x3 & value)
{
    glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::mat3x2 & value)
{
    glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::mat2x4 & value)
{
    glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::mat4x2 & value)
{
    glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::mat3x4 & value)
{
    glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const glm::mat4x3 & value)
{
    glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const Array<float> & value)
{
    glUniform1fv(location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<int> & value)
{
    glUniform1iv(location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<unsigned int> & value)
{
    glUniform1uiv(location, static_cast<GLint>(value.size()), value.data());
    CheckGLError();
}

void UniformSetter::set(const GLint location, const Array<glm::vec2> & value)
{
    glUniform2fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::vec3> & value)
{
    glUniform3fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::vec4> & value)
{
    glUniform4fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const Array<glm::ivec2> & value)
{
    glUniform2iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::ivec3> & value)
{
    glUniform3iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::ivec4> & value)
{
    glUniform4iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.rawData()));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const Array<glm::uvec2> & value)
{
    glUniform2uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::uvec3> & value)
{
    glUniform3uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::uvec4> & value)
{
    glUniform4uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.rawData()));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const Array<glm::mat2> & value)
{
    glUniformMatrix2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::mat3> & value)
{
    glUniformMatrix3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::mat4> & value)
{
    glUniformMatrix2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const Array<glm::mat2x3> & value)
{
    glUniformMatrix2x3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::mat3x2> & value)
{
    glUniformMatrix3x2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::mat2x4> & value)
{
    glUniformMatrix2x4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::mat4x2> & value)
{
    glUniformMatrix4x2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::mat3x4> & value)
{
    glUniformMatrix3x4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<glm::mat4x3> & value)
{
    glUniformMatrix4x3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
    CheckGLError();
}

void UniformSetter::set(const GLint location, const TextureHandle & value)
{
    glUniformHandleui64ARB(location, value);
    CheckGLError();
}
void UniformSetter::set(const GLint location, const Array<TextureHandle> & value)
{
    const TextureHandle * handle = value.data();
    glUniformHandleui64vARB(location, static_cast<GLint>(value.size()), handle);
    CheckGLError();
}

} // namespace glow
