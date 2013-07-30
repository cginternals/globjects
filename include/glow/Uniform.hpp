#pragma once

#include <glm/glm.hpp>

#include <glow/Log.h>
#include <glow/Array.hpp>
#include <glow/Texture.h>

namespace glow 
{


class TemplateError {
	private:
		TemplateError();
};


template<typename T>
Uniform<T>::Uniform(const std::string & name)
:	Uniform(name, T())
{
}

template<typename T>
Uniform<T>::Uniform(const std::string & name, const T & value)
:	AbstractUniform(name)
,	_value(value)
{
}

template<typename T>
Uniform<T>::~Uniform()
{
}

template<typename T>
void Uniform<T>::update(Program * program)
{
	program->use();
	set(program->getUniformLocation(_name), _value);
}

template<typename T>
void Uniform<T>::set(const T & value)
{
	_value = value;
	changed();
}

template<typename T>
void Uniform<T>::set(const int location, const T & value)
{
	TemplateError e;
}

template<> void Uniform<float>::set(const int location, const float& value) 
{
	glUniform1f(location, value); 
}
template<> void Uniform<int>::set(const int location, const int & value)
{
	glUniform1i(location, value); 
}
template<> void Uniform<unsigned int>::set(const int location, const unsigned int & value)
{
	glUniform1ui(location, value); 
}

template<> void Uniform<glm::vec2>::set(const int location, const glm::vec2 & value) 
{
	glUniform2fv(location, 1, glm::value_ptr(value)); 
}
template<> void Uniform<glm::vec3>::set(const int location, const glm::vec3 & value) 
{
	glUniform3fv(location, 1, glm::value_ptr(value)); 
}
template<> void Uniform<glm::vec4> ::set(const int location, const glm::vec4 & value) 
{
	glUniform4fv(location, 1, glm::value_ptr(value)); 
}

template<> void Uniform<glm::ivec2>::set(const int location, const glm::ivec2 & value) 
{
	glUniform2iv(location, 1, glm::value_ptr(value)); 
}
template<> void Uniform<glm::ivec3>::set(const int location, const glm::ivec3 & value) 
{
	glUniform3iv(location, 1, glm::value_ptr(value)); 
}
template<> void Uniform<glm::ivec4>::set(const int location, const glm::ivec4 & value)
{
	glUniform4iv(location, 1, glm::value_ptr(value));
}

template<> void Uniform<glm::uvec2>::set(const int location, const glm::uvec2 & value) 
{
	glUniform2uiv(location, 1, glm::value_ptr(value)); 
}
template<> void Uniform<glm::uvec3>::set(const int location, const glm::uvec3 & value) 
{
	glUniform3uiv(location, 1, glm::value_ptr(value)); 
}
template<> void Uniform<glm::uvec4>::set(const int location, const glm::uvec4 & value) 
{
	glUniform4uiv(location, 1, glm::value_ptr(value)); 
}

template<> void Uniform<glm::mat2> ::set(const int location, const glm::mat2 & value) 
{
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
template<> void Uniform<glm::mat3> ::set(const int location, const glm::mat3 & value) 
{
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)); 
}
template<> void Uniform<glm::mat4> ::set(const int location, const glm::mat4 & value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)); 
}

template<> void Uniform<glm::mat2x3>::set(const int location, const glm::mat2x3 & value) 
{
	glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value)); 
}
template<> void Uniform<glm::mat3x2>::set(const int location, const glm::mat3x2 & value) 
{
	glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
template<> void Uniform<glm::mat2x4>::set(const int location, const glm::mat2x4 & value)
{
	glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value)); 
}
template<> void Uniform<glm::mat4x2>::set(const int location, const glm::mat4x2 & value) 
{
	glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value)); 
}
template<> void Uniform<glm::mat3x4>::set(const int location, const glm::mat3x4 & value)
{
	glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value)); 
}
template<> void Uniform<glm::mat4x3>::set(const int location, const glm::mat4x3 & value)
{
	glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value)); 
}
		
template<> void Uniform<Array<float>>::set(const int location, const Array<float> & value) 
{
	glUniform1fv(location, value.size(), value.data()); 
}
template<> void Uniform<Array<int>>::set(const int location, const Array<int> & value)
{
	glUniform1iv(location, value.size(), value.data()); 
}
template<> void Uniform<Array<unsigned int>>::set(const int location, const Array<unsigned int> & value)
{
	glUniform1uiv(location, value.size(), value.data()); 
}

template<> void Uniform<Array<glm::vec2>>::set(const int location, const Array<glm::vec2> & value) 
{
	glUniform2fv(location, value.size(), reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::vec3>>::set(const int location, const Array<glm::vec3> & value) 
{
	glUniform3fv(location, value.size(), reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::vec4>>::set(const int location, const Array<glm::vec4> & value) 
{
	glUniform4fv(location, value.size(), reinterpret_cast<const float*>(value.rawData())); 
}

template<> void Uniform<Array<glm::ivec2>>::set(const int location, const Array<glm::ivec2> & value) 
{
	glUniform2iv(location, value.size(), reinterpret_cast<const int*>(value.rawData())); 
}
template<> void Uniform<Array<glm::ivec3>>::set(const int location, const Array<glm::ivec3> & value) 
{
	glUniform3iv(location, value.size(), reinterpret_cast<const int*>(value.rawData())); 
}
template<> void Uniform<Array<glm::ivec4>>::set(const int location, const Array<glm::ivec4> & value) 
{
	glUniform4iv(location, value.size(), reinterpret_cast<const int*>(value.rawData())); 
}

template<> void Uniform<Array<glm::uvec2>>::set(const int location, const Array<glm::uvec2> & value) 
{
	glUniform2uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData())); 
}
template<> void Uniform<Array<glm::uvec3>>::set(const int location, const Array<glm::uvec3> & value) 
{
	glUniform3uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData())); 
}
template<> void Uniform<Array<glm::uvec4>>::set(const int location, const Array<glm::uvec4> & value) 
{
	glUniform4uiv(location, value.size(), reinterpret_cast<const unsigned*>(value.rawData())); 
}

template<> void Uniform<Array<glm::mat2>>::set(const int location, const Array<glm::mat2> & value) 
{
	glUniformMatrix2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::mat3>>::set(const int location, const Array<glm::mat3> & value) 
{
	glUniformMatrix3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::mat4>>::set(const int location, const Array<glm::mat4> & value) 
{
	glUniformMatrix2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}

template<> void Uniform<Array<glm::mat2x3>>::set(const int location, const Array<glm::mat2x3> & value) 
{
	glUniformMatrix2x3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::mat3x2>>::set(const int location, const Array<glm::mat3x2> & value) 
{
	glUniformMatrix3x2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::mat2x4>>::set(const int location, const Array<glm::mat2x4> & value) 
{
	glUniformMatrix2x4fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::mat4x2>>::set(const int location, const Array<glm::mat4x2> & value) 
{
	glUniformMatrix4x2fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::mat3x4>>::set(const int location, const Array<glm::mat3x4> & value) 
{
	glUniformMatrix3x4fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}
template<> void Uniform<Array<glm::mat4x3>>::set(const int location, const Array<glm::mat4x3> & value) 
{
	glUniformMatrix4x3fv(location, value.size(), GL_FALSE, reinterpret_cast<const float*>(value.rawData())); 
}

#ifdef GL_NV_bindless_texture
template<> void Uniform<Texture::Handle>::set(const int location, const Texture::Handle & value)
{
	glUniformHandleui64NV(location, value.value); 
}
template<> void Uniform<glow::Array<Texture::Handle>>::set(const int location, const Array<Texture::Handle> & value)
{
	const Texture::Handle * handle = value.data();
	glUniformHandleui64vNV(location, value.size(), reinterpret_cast<const GLuint64 *>(handle));
}
#endif

} // namespace glow