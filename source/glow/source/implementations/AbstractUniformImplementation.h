#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <glow/TextureHandle.h>

namespace glow
{

class Program;

class AbstractUniformImplementation
{
public:
    AbstractUniformImplementation();
    virtual ~AbstractUniformImplementation();

    static AbstractUniformImplementation * create();

    virtual void set(const Program * program, gl::GLint location, const float & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const int & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const unsigned int & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const bool & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const glm::vec2 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::vec3 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::vec4 & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const glm::ivec2 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::ivec3 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::ivec4 & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const glm::uvec2 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::uvec3 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::uvec4 & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const glm::mat2 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::mat3 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::mat4 & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const glm::mat2x3 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::mat3x2 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::mat2x4 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::mat4x2 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::mat3x4 & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const glm::mat4x3 & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const TextureHandle & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const std::vector<float> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<int> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<unsigned int> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<bool> & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::vec2> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::vec3> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::vec4> & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::ivec2> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::ivec3> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::ivec4> & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::uvec2> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::uvec3> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::uvec4> & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat2> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat3> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat4> & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat2x3> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat3x2> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat2x4> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat4x2> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat3x4> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<glm::mat4x3> & value) const = 0;

    virtual void set(const Program * program, gl::GLint location, const std::vector<TextureHandle> & value) const = 0;
};

} // namespace glow
