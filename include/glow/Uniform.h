#pragma once

#include <glow/declspec.h>

#include <glow/Referenced.h>
#include <glow/Array.hpp>
#include <glow/internal/UniformSetter.hpp>
#include <glm/glm.hpp>

#include <string>
#include <set>

#define SETTER(type) \
	void set(const type& value) {\
		setUniformSetter(new internal::UniformSetter<type>(value));\
		changed();\
	}

namespace glow {

class Program;

class GLOW_API Uniform : public Referenced
{
	friend class Program;
public:
	Uniform(const std::string& name);
	~Uniform();

	const std::string& name() const;

        void registerProgram(Program* program);
        void deregisterProgram(Program* program);

	SETTER(float);
	SETTER(int);
	SETTER(unsigned int);
	SETTER(glm::vec2);
	SETTER(glm::vec3);
	SETTER(glm::vec4);
	SETTER(glm::uvec2);
	SETTER(glm::uvec3);
	SETTER(glm::uvec4);
	SETTER(glm::mat2);
	SETTER(glm::mat3);
	SETTER(glm::mat4);
	SETTER(glm::mat2x3);
	SETTER(glm::mat3x2);
	SETTER(glm::mat2x4);
	SETTER(glm::mat4x2);
	SETTER(glm::mat3x4);
	SETTER(glm::mat4x3);

	SETTER(Array<float>);
	SETTER(Array<int>);
	SETTER(Array<unsigned>);
	SETTER(Array<glm::vec2>);
	SETTER(Array<glm::vec3>);
	SETTER(Array<glm::vec4>);
	SETTER(Array<glm::uvec2>);
	SETTER(Array<glm::uvec3>);
	SETTER(Array<glm::uvec4>);
	SETTER(Array<glm::mat2>);
	SETTER(Array<glm::mat3>);
	SETTER(Array<glm::mat4>);
	SETTER(Array<glm::mat2x3>);
	SETTER(Array<glm::mat3x2>);
	SETTER(Array<glm::mat2x4>);
	SETTER(Array<glm::mat4x2>);
	SETTER(Array<glm::mat3x4>);
	SETTER(Array<glm::mat4x3>);

protected:
        std::string _name;
        internal::AbstractUniformSetter* _setter;
        std::set<Program*> _programs;

        void setUniformSetter(internal::AbstractUniformSetter* setter);

	void changed();
	void setFor(Program* program);
        void setUniform(GLint location);
};

} // namespace glow
