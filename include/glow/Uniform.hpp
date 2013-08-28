#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <glow/Array.h>
#include <glow/Texture.h>

namespace glow
{

class TemplateError 
{
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
,	m_value(value)
{
}

template<typename T>
Uniform<T>::~Uniform()
{
}

template<typename T>
const T & Uniform<T>::value() const
{
	return m_value;
}

template<typename T>
void Uniform<T>::setLocation(const GLint location)
{
	set(location, m_value);
}

template<typename T>
void Uniform<T>::set(const T & value)
{
	m_value = value;
	changed();
}

template<typename T>
void Uniform<T>::set(const GLint location, const T & value)
{
	TemplateError e;
}

template<> void Uniform<float>::set(const GLint location, const float& value);
template<> void Uniform<int>::set(const GLint location, const int & value);
template<> void Uniform<unsigned int>::set(const GLint location, const unsigned int & value);
template<> void Uniform<bool>::set(const GLint location, const bool & value);

template<> void Uniform<glm::vec2>::set(const GLint location, const glm::vec2 & value);
template<> void Uniform<glm::vec3>::set(const GLint location, const glm::vec3 & value);
template<> void Uniform<glm::vec4> ::set(const GLint location, const glm::vec4 & value);

template<> void Uniform<glm::ivec2>::set(const GLint location, const glm::ivec2 & value);
template<> void Uniform<glm::ivec3>::set(const GLint location, const glm::ivec3 & value);
template<> void Uniform<glm::ivec4>::set(const GLint location, const glm::ivec4 & value);

template<> void Uniform<glm::uvec2>::set(const GLint location, const glm::uvec2 & value);
template<> void Uniform<glm::uvec3>::set(const GLint location, const glm::uvec3 & value);
template<> void Uniform<glm::uvec4>::set(const GLint location, const glm::uvec4 & value);

template<> void Uniform<glm::mat2>::set(const GLint location, const glm::mat2 & value);
template<> void Uniform<glm::mat3>::set(const GLint location, const glm::mat3 & value);
template<> void Uniform<glm::mat4>::set(const GLint location, const glm::mat4 & value);

template<> void Uniform<glm::mat2x3>::set(const GLint location, const glm::mat2x3 & value);
template<> void Uniform<glm::mat3x2>::set(const GLint location, const glm::mat3x2 & value);
template<> void Uniform<glm::mat2x4>::set(const GLint location, const glm::mat2x4 & value);
template<> void Uniform<glm::mat4x2>::set(const GLint location, const glm::mat4x2 & value);
template<> void Uniform<glm::mat3x4>::set(const GLint location, const glm::mat3x4 & value);
template<> void Uniform<glm::mat4x3>::set(const GLint location, const glm::mat4x3 & value);

template<> void Uniform<Array<float>>::set(const GLint location, const Array<float> & value);
template<> void Uniform<Array<int>>::set(const GLint location, const Array<int> & value);
template<> void Uniform<Array<unsigned int>>::set(const GLint location, const Array<unsigned int> & value);

template<> void Uniform<Array<glm::vec2>>::set(const GLint location, const Array<glm::vec2> & value);
template<> void Uniform<Array<glm::vec3>>::set(const GLint location, const Array<glm::vec3> & value);
template<> void Uniform<Array<glm::vec4>>::set(const GLint location, const Array<glm::vec4> & value);

template<> void Uniform<Array<glm::ivec2>>::set(const GLint location, const Array<glm::ivec2> & value);
template<> void Uniform<Array<glm::ivec3>>::set(const GLint location, const Array<glm::ivec3> & value);
template<> void Uniform<Array<glm::ivec4>>::set(const GLint location, const Array<glm::ivec4> & value);

template<> void Uniform<Array<glm::uvec2>>::set(const GLint location, const Array<glm::uvec2> & value);
template<> void Uniform<Array<glm::uvec3>>::set(const GLint location, const Array<glm::uvec3> & value);
template<> void Uniform<Array<glm::uvec4>>::set(const GLint location, const Array<glm::uvec4> & value);

template<> void Uniform<Array<glm::mat2>>::set(const GLint location, const Array<glm::mat2> & value);
template<> void Uniform<Array<glm::mat3>>::set(const GLint location, const Array<glm::mat3> & value);
template<> void Uniform<Array<glm::mat4>>::set(const GLint location, const Array<glm::mat4> & value);

template<> void Uniform<Array<glm::mat2x3>>::set(const GLint location, const Array<glm::mat2x3> & value);
template<> void Uniform<Array<glm::mat3x2>>::set(const GLint location, const Array<glm::mat3x2> & value);
template<> void Uniform<Array<glm::mat2x4>>::set(const GLint location, const Array<glm::mat2x4> & value);
template<> void Uniform<Array<glm::mat4x2>>::set(const GLint location, const Array<glm::mat4x2> & value);
template<> void Uniform<Array<glm::mat3x4>>::set(const GLint location, const Array<glm::mat3x4> & value);
template<> void Uniform<Array<glm::mat4x3>>::set(const GLint location, const Array<glm::mat4x3> & value);

template<> void Uniform<Texture::Handle>::set(const GLint location, const Texture::Handle & value);
template<> void Uniform<glow::Array<Texture::Handle>>::set(const GLint location, const Array<Texture::Handle> & value);

} // namespace glow
