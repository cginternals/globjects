#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <glow/glow.h>
#include <glow/AbstractUniform.h>
#include <glow/TextureHandle.h>

namespace glow
{
template <typename T>
class Array;


class GLOW_API UniformSetter
{
public:
    static void set(GLint location, const float & value);
    static void set(GLint location, const int & value);
    static void set(GLint location, const unsigned int & value);
    static void set(GLint location, const bool & value);

    static void set(GLint location, const glm::vec2 & value);
    static void set(GLint location, const glm::vec3 & value);
    static void set(GLint location, const glm::vec4 & value);

    static void set(GLint location, const glm::ivec2 & value);
    static void set(GLint location, const glm::ivec3 & value);
    static void set(GLint location, const glm::ivec4 & value);

    static void set(GLint location, const glm::uvec2 & value);
    static void set(GLint location, const glm::uvec3 & value);
    static void set(GLint location, const glm::uvec4 & value);

    static void set(GLint location, const glm::mat2 & value);
    static void set(GLint location, const glm::mat3 & value);
    static void set(GLint location, const glm::mat4 & value);

    static void set(GLint location, const glm::mat2x3 & value);
    static void set(GLint location, const glm::mat3x2 & value);
    static void set(GLint location, const glm::mat2x4 & value);
    static void set(GLint location, const glm::mat4x2 & value);
    static void set(GLint location, const glm::mat3x4 & value);
    static void set(GLint location, const glm::mat4x3 & value);

    static void set(GLint location, const Array<float> & value);
    static void set(GLint location, const Array<int> & value);
    static void set(GLint location, const Array<unsigned int> & value);

    static void set(GLint location, const Array<glm::vec2> & value);
    static void set(GLint location, const Array<glm::vec3> & value);
    static void set(GLint location, const Array<glm::vec4> & value);

    static void set(GLint location, const Array<glm::ivec2> & value);
    static void set(GLint location, const Array<glm::ivec3> & value);
    static void set(GLint location, const Array<glm::ivec4> & value);

    static void set(GLint location, const Array<glm::uvec2> & value);
    static void set(GLint location, const Array<glm::uvec3> & value);
    static void set(GLint location, const Array<glm::uvec4> & value);

    static void set(GLint location, const Array<glm::mat2> & value);
    static void set(GLint location, const Array<glm::mat3> & value);
    static void set(GLint location, const Array<glm::mat4> & value);

    static void set(GLint location, const Array<glm::mat2x3> & value);
    static void set(GLint location, const Array<glm::mat3x2> & value);
    static void set(GLint location, const Array<glm::mat2x4> & value);
    static void set(GLint location, const Array<glm::mat4x2> & value);
    static void set(GLint location, const Array<glm::mat3x4> & value);
    static void set(GLint location, const Array<glm::mat4x3> & value);

    static void set(GLint location, const TextureHandle & value);
    static void set(GLint location, const Array<TextureHandle> & value);
};

} // namespace glow

