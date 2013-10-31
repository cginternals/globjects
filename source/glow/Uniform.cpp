
#include <glow/Error.h>

#include <glow/Uniform.h>

namespace glow
{

template<> void Uniform<float>::set(const GLint location, const float& value)
{
	glUniform1f(location, value);
	CheckGLError();
}
template<> void Uniform<int>::set(const GLint location, const int & value)
{
	glUniform1i(location, value);
	CheckGLError();
}
template<> void Uniform<unsigned int>::set(const GLint location, const unsigned int & value)
{
	glUniform1ui(location, value);
	CheckGLError();
}
template<> void Uniform<bool>::set(const GLint location, const bool & value)
{
	glUniform1i(location, value ? 1 : 0);
	CheckGLError();
}

template<> void Uniform<glm::vec2>::set(const GLint location, const glm::vec2 & value)
{
	glUniform2fv(location, 1, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::vec3>::set(const GLint location, const glm::vec3 & value)
{
	glUniform3fv(location, 1, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::vec4> ::set(const GLint location, const glm::vec4 & value)
{
	glUniform4fv(location, 1, glm::value_ptr(value));
	CheckGLError();
}

template<> void Uniform<glm::ivec2>::set(const GLint location, const glm::ivec2 & value)
{
	glUniform2iv(location, 1, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::ivec3>::set(const GLint location, const glm::ivec3 & value)
{
	glUniform3iv(location, 1, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::ivec4>::set(const GLint location, const glm::ivec4 & value)
{
	glUniform4iv(location, 1, glm::value_ptr(value));
	CheckGLError();
}

template<> void Uniform<glm::uvec2>::set(const GLint location, const glm::uvec2 & value)
{
	glUniform2uiv(location, 1, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::uvec3>::set(const GLint location, const glm::uvec3 & value)
{
	glUniform3uiv(location, 1, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::uvec4>::set(const GLint location, const glm::uvec4 & value)
{
	glUniform4uiv(location, 1, glm::value_ptr(value));
	CheckGLError();
}

template<> void Uniform<glm::mat2> ::set(const GLint location, const glm::mat2 & value)
{
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::mat3> ::set(const GLint location, const glm::mat3 & value)
{
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::mat4> ::set(const GLint location, const glm::mat4 & value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}

template<> void Uniform<glm::mat2x3>::set(const GLint location, const glm::mat2x3 & value)
{
	glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::mat3x2>::set(const GLint location, const glm::mat3x2 & value)
{
	glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::mat2x4>::set(const GLint location, const glm::mat2x4 & value)
{
	glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::mat4x2>::set(const GLint location, const glm::mat4x2 & value)
{
	glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::mat3x4>::set(const GLint location, const glm::mat3x4 & value)
{
	glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}
template<> void Uniform<glm::mat4x3>::set(const GLint location, const glm::mat4x3 & value)
{
	glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	CheckGLError();
}

template<> void Uniform<Array<float>>::set(const GLint location, const Array<float> & value)
{
	glUniform1fv(location, value.size(), value.data());
	CheckGLError();
}
template<> void Uniform<Array<int>>::set(const GLint location, const Array<int> & value)
{
	glUniform1iv(location, value.size(), value.data());
	CheckGLError();
}
template<> void Uniform<Array<unsigned int>>::set(const GLint location, const Array<unsigned int> & value)
{
	glUniform1uiv(location, value.size(), value.data());
	CheckGLError();
}

template<> void Uniform<Array<glm::vec2>>::set(const GLint location, const Array<glm::vec2> & value)
{
	glUniform2fv(location, value.size(), reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::vec3>>::set(const GLint location, const Array<glm::vec3> & value)
{
	glUniform3fv(location, value.size(), reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::vec4>>::set(const GLint location, const Array<glm::vec4> & value)
{
	glUniform4fv(location, value.size(), reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}

template<> void Uniform<Array<glm::ivec2>>::set(const GLint location, const Array<glm::ivec2> & value)
{
	glUniform2iv(location, value.size(), reinterpret_cast<const int*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::ivec3>>::set(const GLint location, const Array<glm::ivec3> & value)
{
	glUniform3iv(location, value.size(), reinterpret_cast<const int*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::ivec4>>::set(const GLint location, const Array<glm::ivec4> & value)
{
	glUniform4iv(location, value.size(), reinterpret_cast<const int*>(value.rawData()));
	CheckGLError();
}

template<> void Uniform<Array<glm::uvec2>>::set(const GLint location, const Array<glm::uvec2> & value)
{
	glUniform2uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::uvec3>>::set(const GLint location, const Array<glm::uvec3> & value)
{
	glUniform3uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::uvec4>>::set(const GLint location, const Array<glm::uvec4> & value)
{
	glUniform4uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData()));
	CheckGLError();
}

template<> void Uniform<Array<glm::mat2>>::set(const GLint location, const Array<glm::mat2> & value)
{
	glUniformMatrix2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::mat3>>::set(const GLint location, const Array<glm::mat3> & value)
{
	glUniformMatrix3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::mat4>>::set(const GLint location, const Array<glm::mat4> & value)
{
	glUniformMatrix2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}

template<> void Uniform<Array<glm::mat2x3>>::set(const GLint location, const Array<glm::mat2x3> & value)
{
	glUniformMatrix2x3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::mat3x2>>::set(const GLint location, const Array<glm::mat3x2> & value)
{
	glUniformMatrix3x2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::mat2x4>>::set(const GLint location, const Array<glm::mat2x4> & value)
{
	glUniformMatrix2x4fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::mat4x2>>::set(const GLint location, const Array<glm::mat4x2> & value)
{
	glUniformMatrix4x2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::mat3x4>>::set(const GLint location, const Array<glm::mat3x4> & value)
{
	glUniformMatrix3x4fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}
template<> void Uniform<Array<glm::mat4x3>>::set(const GLint location, const Array<glm::mat4x3> & value)
{
	glUniformMatrix4x3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData()));
	CheckGLError();
}

template<> void Uniform<Texture::Handle>::set(const GLint location, const Texture::Handle & value)
{
	glUniformHandleui64NV(location, value.value);
	CheckGLError();
}
template<> void Uniform<glow::Array<Texture::Handle>>::set(const GLint location, const Array<Texture::Handle> & value)
{
	const Texture::Handle * handle = value.data();
	glUniformHandleui64vNV(location, value.size(), reinterpret_cast<const GLuint64 *>(handle));
	CheckGLError();
}

} // namespace glow
