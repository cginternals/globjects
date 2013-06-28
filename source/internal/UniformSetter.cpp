#include <glow/internal/UniformSetter.hpp>

namespace glow {
namespace internal {

template<> void setUniform<float>(int location, const float& value) { glUniform1f(location, value); }
template<> void setUniform<int>(int location, const int& value) { glUniform1i(location, value); }
template<> void setUniform<unsigned int>(int location, const unsigned int& value) { glUniform1ui(location, value); }
template<> void setUniform<glm::vec2>(int location, const glm::vec2& value) { glUniform2fv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::vec3>(int location, const glm::vec3& value) { glUniform3fv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::vec4>(int location, const glm::vec4& value) { glUniform4fv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::ivec2>(int location, const glm::ivec2& value) { glUniform2iv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::ivec3>(int location, const glm::ivec3& value) { glUniform3iv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::ivec4>(int location, const glm::ivec4& value) { glUniform4iv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::uvec2>(int location, const glm::uvec2& value) { glUniform2uiv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::uvec3>(int location, const glm::uvec3& value) { glUniform3uiv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::uvec4>(int location, const glm::uvec4& value) { glUniform4uiv(location, 1, glm::value_ptr(value)); }
template<> void setUniform<glm::mat2>(int location, const glm::mat2& value) { glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat3>(int location, const glm::mat3& value) { glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat4>(int location, const glm::mat4& value) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat2x3>(int location, const glm::mat2x3& value) { glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat3x2>(int location, const glm::mat3x2& value) { glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat2x4>(int location, const glm::mat2x4& value) { glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat4x2>(int location, const glm::mat4x2& value) { glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat3x4>(int location, const glm::mat3x4& value) { glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glm::mat4x3>(int location, const glm::mat4x3& value) { glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
template<> void setUniform<glow::Array<float>>(int location, const glow::Array<float>& value) { glUniform1fv(location, value.size(), value.data()); }
template<> void setUniform<glow::Array<int>>(int location, const glow::Array<int>& value) { glUniform1iv(location, value.size(), value.data()); }
template<> void setUniform<glow::Array<unsigned int>>(int location, const glow::Array<unsigned int>& value) { glUniform1uiv(location, value.size(), value.data()); }
template<> void setUniform<glow::Array<glm::vec2>>(int location, const glow::Array<glm::vec2>& value) { glUniform2fv(location, value.size(), reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::vec3>>(int location, const glow::Array<glm::vec3>& value) { glUniform3fv(location, value.size(), reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::vec4>>(int location, const glow::Array<glm::vec4>& value) { glUniform4fv(location, value.size(), reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::ivec2>>(int location, const glow::Array<glm::ivec2>& value) { glUniform2iv(location, value.size(), reinterpret_cast<const int*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::ivec3>>(int location, const glow::Array<glm::ivec3>& value) { glUniform3iv(location, value.size(), reinterpret_cast<const int*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::ivec4>>(int location, const glow::Array<glm::ivec4>& value) { glUniform4iv(location, value.size(), reinterpret_cast<const int*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::uvec2>>(int location, const glow::Array<glm::uvec2>& value) { glUniform2uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::uvec3>>(int location, const glow::Array<glm::uvec3>& value) { glUniform3uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::uvec4>>(int location, const glow::Array<glm::uvec4>& value) { glUniform4uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat2>>(int location, const glow::Array<glm::mat2>& value) { glUniformMatrix2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat3>>(int location, const glow::Array<glm::mat3>& value) { glUniformMatrix3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat4>>(int location, const glow::Array<glm::mat4>& value) { glUniformMatrix2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat2x3>>(int location, const glow::Array<glm::mat2x3>& value) { glUniformMatrix2x3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat3x2>>(int location, const glow::Array<glm::mat3x2>& value) { glUniformMatrix3x2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat2x4>>(int location, const glow::Array<glm::mat2x4>& value) { glUniformMatrix2x4fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat4x2>>(int location, const glow::Array<glm::mat4x2>& value) { glUniformMatrix4x2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat3x4>>(int location, const glow::Array<glm::mat3x4>& value) { glUniformMatrix3x4fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }
template<> void setUniform<glow::Array<glm::mat4x3>>(int location, const glow::Array<glm::mat4x3>& value) { glUniformMatrix4x3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); }

} // namespace internal
} // namespace glow
