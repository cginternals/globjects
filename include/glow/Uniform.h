#pragma once

#include <glow/declspec.h>

#include <glow/Referenced.h>
#include <glow/Array.hpp>
#include <glm/glm.hpp>

#include <string>
#include <set>

#define UNIFORM_SET(type, type_name, member) \
	void set(const type& value) {\
		_type = type_name;\
		_value.member = value;\
		update();\
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

	UNIFORM_SET(float, Float, float_value);
	UNIFORM_SET(int, Int, int_value);
	UNIFORM_SET(unsigned, UInt, uint_value);
	UNIFORM_SET(glm::vec2, Vec2, vec2_value);
	UNIFORM_SET(glm::vec3, Vec3, vec3_value);
	UNIFORM_SET(glm::vec4, Vec4, vec4_value);
	UNIFORM_SET(glm::uvec2, UVec2, uvec2_value);
	UNIFORM_SET(glm::uvec3, UVec3, uvec3_value);
	UNIFORM_SET(glm::uvec4, UVec4, uvec4_value);
	UNIFORM_SET(glm::mat2, Mat2, mat2_value);
	UNIFORM_SET(glm::mat3, Mat3, mat3_value);
	UNIFORM_SET(glm::mat4, Mat4, mat4_value);
	UNIFORM_SET(glm::mat2x3, Mat2x3, mat2x3_value);
	UNIFORM_SET(glm::mat3x2, Mat3x2, mat3x2_value);
	UNIFORM_SET(glm::mat2x4, Mat2x4, mat2x4_value);
	UNIFORM_SET(glm::mat4x2, Mat4x2, mat4x2_value);
	UNIFORM_SET(glm::mat3x4, Mat3x4, mat3x4_value);
	UNIFORM_SET(glm::mat4x3, Mat4x3, mat4x3_value);

	UNIFORM_SET(Array<float>, FloatArray, float_array_value);
	UNIFORM_SET(Array<int>, IntArray, int_array_value);
	UNIFORM_SET(Array<unsigned>, UIntArray, uint_array_value);
	UNIFORM_SET(Array<glm::vec2>, Vec2Array, vec2_array_value);
	UNIFORM_SET(Array<glm::vec3>, Vec3Array, vec3_array_value);
	UNIFORM_SET(Array<glm::vec4>, Vec4Array, vec4_array_value);
	UNIFORM_SET(Array<glm::uvec2>, UVec2Array, uvec2_array_value);
	UNIFORM_SET(Array<glm::uvec3>, UVec3Array, uvec3_array_value);
	UNIFORM_SET(Array<glm::uvec4>, UVec4Array, uvec4_array_value);
	UNIFORM_SET(Array<glm::mat2>, Mat2Array, mat2_array_value);
	UNIFORM_SET(Array<glm::mat3>, Mat3Array, mat3_array_value);
	UNIFORM_SET(Array<glm::mat4>, Mat4Array, mat4_array_value);
	UNIFORM_SET(Array<glm::mat2x3>, Mat2x3Array, mat2x3_array_value);
	UNIFORM_SET(Array<glm::mat3x2>, Mat3x2Array, mat3x2_array_value);
	UNIFORM_SET(Array<glm::mat2x4>, Mat2x4Array, mat2x4_array_value);
	UNIFORM_SET(Array<glm::mat4x2>, Mat4x2Array, mat4x2_array_value);
	UNIFORM_SET(Array<glm::mat3x4>, Mat3x4Array, mat3x4_array_value);
	UNIFORM_SET(Array<glm::mat4x3>, Mat4x3Array, mat4x3_array_value);

protected:
        enum Type
        {
        	Undefined,
        	Float,
		Int,
		UInt,
        	Vec2,
        	Vec3,
        	Vec4,
        	IVec2,
        	IVec3,
        	IVec4,
        	UVec2,
        	UVec3,
        	UVec4,
        	Mat2,
        	Mat3,
        	Mat4,
        	Mat2x3,
        	Mat3x2,
        	Mat2x4,
        	Mat4x2,
        	Mat3x4,
        	Mat4x3,
        	FloatArray,
		IntArray,
		UIntArray,
        	Vec2Array,
        	Vec3Array,
        	Vec4Array,
        	IVec2Array,
        	IVec3Array,
        	IVec4Array,
        	UVec2Array,
        	UVec3Array,
        	UVec4Array,
        	Mat2Array,
        	Mat3Array,
        	Mat4Array,
        	Mat2x3Array,
        	Mat3x2Array,
        	Mat2x4Array,
        	Mat4x2Array,
        	Mat3x4Array,
        	Mat4x3Array
        };
	union Value
        {
        	Value() {}
		~Value() {}

		float float_value;
		int int_value;
		unsigned int uint_value;
		glm::vec2 vec2_value;
		glm::vec3 vec3_value;
		glm::vec4 vec4_value;
		glm::ivec2 ivec2_value;
		glm::ivec3 ivec3_value;
		glm::ivec4 ivec4_value;
		glm::uvec2 uvec2_value;
		glm::uvec3 uvec3_value;
		glm::uvec4 uvec4_value;
		glm::mat2 mat2_value;
		glm::mat3 mat3_value;
		glm::mat4 mat4_value;
		glm::mat2x3 mat2x3_value;
		glm::mat3x2 mat3x2_value;
		glm::mat2x4 mat2x4_value;
		glm::mat4x2 mat4x2_value;
		glm::mat3x4 mat3x4_value;
		glm::mat4x3 mat4x3_value;

		glow::Array<float> float_array_value;
		glow::Array<int> int_array_value;
		glow::Array<unsigned int> uint_array_value;
		glow::Array<glm::vec2> vec2_array_value;
		glow::Array<glm::vec3> vec3_array_value;
		glow::Array<glm::vec4> vec4_array_value;
		glow::Array<glm::ivec2> ivec2_array_value;
		glow::Array<glm::ivec3> ivec3_array_value;
		glow::Array<glm::ivec4> ivec4_array_value;
		glow::Array<glm::uvec2> uvec2_array_value;
		glow::Array<glm::uvec3> uvec3_array_value;
		glow::Array<glm::uvec4> uvec4_array_value;
		glow::Array<glm::mat2> mat2_array_value;
		glow::Array<glm::mat3> mat3_array_value;
		glow::Array<glm::mat4> mat4_array_value;
		glow::Array<glm::mat2x3> mat2x3_array_value;
		glow::Array<glm::mat3x2> mat3x2_array_value;
		glow::Array<glm::mat2x4> mat2x4_array_value;
		glow::Array<glm::mat4x2> mat4x2_array_value;
		glow::Array<glm::mat3x4> mat3x4_array_value;
		glow::Array<glm::mat4x3> mat4x3_array_value;
        };

        Type _type;
        Value _value;
        std::string _name;

        std::set<Program*> _programs;

	void update();
	void updateFor(Program* program);
        void setUniform(GLint location);

        void addTo(Program* program);
        void removeFrom(Program* program);
};

} // namespace glow
