#pragma once

#include <glow/Log.h>
#include <glow/Array.hpp>
#include <glm/glm.hpp>
#include <glow/Texture.h>

#define DECLARE_UNIFORM_SETTER_SPECIALIZATION(type) \
	template<> void setUniform<type>(int location, const type& value);

namespace glow {
namespace internal {

template<typename T>
void setUniform(int location, const T& value)
{
	glow::warning() << "Uniform: type not supported";
}

class AbstractUniformSetter
{
public:
	virtual ~AbstractUniformSetter() {};

	virtual void set(int location) = 0;
};

template<typename T>
class UniformSetter : public AbstractUniformSetter
{
public:
	UniformSetter(const T& value) : value(value) {}

	virtual void set(int location) { setUniform(location, value); }
protected:
	T value;
};

DECLARE_UNIFORM_SETTER_SPECIALIZATION(float);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(int);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(unsigned int);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::vec2);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::vec3);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::vec4);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::uvec2);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::uvec3);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::uvec4);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat2);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat3);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat4);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat2x3);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat3x2);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat2x4);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat4x2);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat3x4);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(glm::mat4x3);

DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<float>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<int>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<unsigned>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::vec2>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::vec3>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::vec4>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::uvec2>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::uvec3>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::uvec4>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat2>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat3>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat4>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat2x3>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat3x2>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat2x4>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat4x2>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat3x4>);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<glm::mat4x3>);

#ifdef GL_NV_bindless_texture
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Texture::Handle);
DECLARE_UNIFORM_SETTER_SPECIALIZATION(Array<Texture::Handle>);
#endif

} // namespace internal
} // namespace glow
