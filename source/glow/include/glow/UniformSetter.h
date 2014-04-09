#pragma once

#include <array>
#include <vector>

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

#include <glow/glow_api.h>
#include <glow/AbstractUniform.h>
#include <glow/TextureHandle.h>

namespace glow
{

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

    static void set(GLint location, const TextureHandle & value);

    static void set(GLint location, const std::vector<float> & value);
    static void set(GLint location, const std::vector<int> & value);
    static void set(GLint location, const std::vector<unsigned int> & value);
    static void set(GLint location, const std::vector<bool> & value);

    static void set(GLint location, const std::vector<glm::vec2> & value);
    static void set(GLint location, const std::vector<glm::vec3> & value);
    static void set(GLint location, const std::vector<glm::vec4> & value);

    static void set(GLint location, const std::vector<glm::ivec2> & value);
    static void set(GLint location, const std::vector<glm::ivec3> & value);
    static void set(GLint location, const std::vector<glm::ivec4> & value);

    static void set(GLint location, const std::vector<glm::uvec2> & value);
    static void set(GLint location, const std::vector<glm::uvec3> & value);
    static void set(GLint location, const std::vector<glm::uvec4> & value);

    static void set(GLint location, const std::vector<glm::mat2> & value);
    static void set(GLint location, const std::vector<glm::mat3> & value);
    static void set(GLint location, const std::vector<glm::mat4> & value);

    static void set(GLint location, const std::vector<glm::mat2x3> & value);
    static void set(GLint location, const std::vector<glm::mat3x2> & value);
    static void set(GLint location, const std::vector<glm::mat2x4> & value);
    static void set(GLint location, const std::vector<glm::mat4x2> & value);
    static void set(GLint location, const std::vector<glm::mat3x4> & value);
    static void set(GLint location, const std::vector<glm::mat4x3> & value);

    static void set(GLint location, const std::vector<TextureHandle> & value);

    template <typename T, std::size_t Count>
    static void set(GLint location, const std::array<T, Count> & value)
    {
        set(location, std::vector<T>(value.data(), value.data()+Count));
    }
};

} // namespace glow
