#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Log.h>
#include <glm/gtc/type_ptr.hpp>


using namespace glow;

Uniform::Uniform(const std::string& name)
: _name(name)
, _type(Undefined)
{
}

Uniform::~Uniform()
{
}

const std::string& Uniform::name() const
{
	return _name;
}

void Uniform::registerProgram(Program* program)
{
	_programs.insert(program);
}

void Uniform::deregisterProgram(Program* program)
{
	_programs.erase(program);
}

void Uniform::changed()
{
	for (Program* program: _programs)
	{
		setFor(program);
	}
}

void Uniform::setFor(Program* program)
{
	program->use();
	setUniform(program->getUniformLocation(_name));
}

void Uniform::setUniform(GLint location)
{
	switch (_type)
	{
        	case Float:
        		glUniform1f(location, _value.float_value);
        		break;
		case Int:
			glUniform1i(location, _value.int_value);
			break;
		case UInt:
			glUniform1ui(location, _value.uint_value);
			break;
        	case Vec2:
        		glUniform2fv(location, 1, glm::value_ptr(_value.vec2_value));
        		break;
        	case Vec3:
        		glUniform3fv(location, 1, glm::value_ptr(_value.vec3_value));
        		break;
        	case Vec4:
        		glUniform3fv(location, 1, glm::value_ptr(_value.vec3_value));
        		break;
        	case IVec2:
        		glUniform2iv(location, 1, glm::value_ptr(_value.ivec2_value));
        		break;
        	case IVec3:
        		glUniform3iv(location, 1, glm::value_ptr(_value.ivec3_value));
        		break;
        	case IVec4:
        		glUniform4iv(location, 1, glm::value_ptr(_value.ivec4_value));
        		break;
        	case UVec2:
        		glUniform2uiv(location, 1, glm::value_ptr(_value.uvec2_value));
        		break;
        	case UVec3:
        		glUniform3uiv(location, 1, glm::value_ptr(_value.uvec3_value));
        		break;
        	case UVec4:
        		glUniform4uiv(location, 1, glm::value_ptr(_value.uvec4_value));
        		break;
        	case Mat2:
        		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat2_value));
        		break;
        	case Mat3:
        		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat3_value));
        		break;
        	case Mat4:
        		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat4_value));
        		break;
        	case Mat2x3:
        		glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat2x3_value));
        		break;
        	case Mat3x2:
        		glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat3x2_value));
        		break;
        	case Mat2x4:
        		glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat2x4_value));
        		break;
        	case Mat4x2:
        		glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat4x2_value));
        		break;
        	case Mat3x4:
        		glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat3x4_value));
        		break;
        	case Mat4x3:
        		glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(_value.mat4x3_value));
        		break;
        	case FloatArray:
        		glUniform1fv(location, _value.float_array_value.size(), _value.float_array_value.data());
        		break;
		case IntArray:
			glUniform1iv(location, _value.int_array_value.size(), _value.int_array_value.data());
			break;
		case UIntArray:
			glUniform1uiv(location, _value.uint_array_value.size(), _value.uint_array_value.data());
			break;
        	case Vec2Array:
        		glUniform2fv(location, _value.vec2_array_value.size(), reinterpret_cast<const float*>(_value.vec2_array_value.rawData()));
        		break;
        	case Vec3Array:
        		glUniform3fv(location, _value.vec3_array_value.size(), reinterpret_cast<const float*>(_value.vec3_array_value.rawData()));
        		break;
        	case Vec4Array:
        		glUniform4fv(location, _value.vec4_array_value.size(), reinterpret_cast<const float*>(_value.vec4_array_value.rawData()));
        		break;
        	case IVec2Array:
        		glUniform2iv(location, _value.ivec2_array_value.size(), reinterpret_cast<const int*>(_value.ivec2_array_value.rawData()));
        		break;
        	case IVec3Array:
        		glUniform3iv(location, _value.ivec3_array_value.size(), reinterpret_cast<const int*>(_value.ivec3_array_value.rawData()));
        		break;
        	case IVec4Array:
        		glUniform4iv(location, _value.ivec4_array_value.size(), reinterpret_cast<const int*>(_value.ivec4_array_value.rawData()));
        		break;
        	case UVec2Array:
        		glUniform2uiv(location, _value.uvec2_array_value.size(), reinterpret_cast<const unsigned*>(_value.uvec2_array_value.rawData()));
        		break;
        	case UVec3Array:
        		glUniform3uiv(location, _value.uvec3_array_value.size(), reinterpret_cast<const unsigned*>(_value.uvec3_array_value.rawData()));
        		break;
        	case UVec4Array:
        		glUniform4uiv(location, _value.uvec4_array_value.size(), reinterpret_cast<const unsigned*>(_value.uvec4_array_value.rawData()));
        		break;
        	case Mat2Array:
        		glUniformMatrix2fv(location, _value.mat2_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat2_array_value.rawData()));
        		break;
        	case Mat3Array:
        		glUniformMatrix3fv(location, _value.mat3_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat3_array_value.rawData()));
        		break;
        	case Mat4Array:
        		glUniformMatrix2fv(location, _value.mat4_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat4_array_value.rawData()));
        		break;
        	case Mat2x3Array:
        		glUniformMatrix2x3fv(location, _value.mat2x3_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat2x3_array_value.rawData()));
        		break;
        	case Mat3x2Array:
        		glUniformMatrix3x2fv(location, _value.mat3x2_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat3x2_array_value.rawData()));
        		break;
        	case Mat2x4Array:
        		glUniformMatrix2x4fv(location, _value.mat2x4_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat2x4_array_value.rawData()));
        		break;
        	case Mat4x2Array:
        		glUniformMatrix4x2fv(location, _value.mat4x2_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat4x2_array_value.rawData()));
        		break;
        	case Mat3x4Array:
        		glUniformMatrix3x4fv(location, _value.mat3x4_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat3x4_array_value.rawData()));
        		break;
        	case Mat4x3Array:
        		glUniformMatrix4x3fv(location, _value.mat4x3_array_value.size(), GL_FALSE, reinterpret_cast<const float*>(_value.mat4x3_array_value.rawData()));
        		break;
		default:
			warning() << "Attempted to set untyped uniform";
	}
}
