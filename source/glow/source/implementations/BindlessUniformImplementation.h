#pragma once

#include "AbstractUniformImplementation.h"

namespace glow
{

class BindlessUniformImplementation : public AbstractUniformImplementation
{
public:
    virtual void set(const Program * program, GLint location, const float & value) const override;
    virtual void set(const Program * program, GLint location, const int & value) const override;
    virtual void set(const Program * program, GLint location, const unsigned int & value) const override;
    virtual void set(const Program * program, GLint location, const bool & value) const override;

    virtual void set(const Program * program, GLint location, const glm::vec2 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::vec3 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::vec4 & value) const override;

    virtual void set(const Program * program, GLint location, const glm::ivec2 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::ivec3 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::ivec4 & value) const override;

    virtual void set(const Program * program, GLint location, const glm::uvec2 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::uvec3 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::uvec4 & value) const override;

    virtual void set(const Program * program, GLint location, const glm::mat2 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::mat3 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::mat4 & value) const override;

    virtual void set(const Program * program, GLint location, const glm::mat2x3 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::mat3x2 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::mat2x4 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::mat4x2 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::mat3x4 & value) const override;
    virtual void set(const Program * program, GLint location, const glm::mat4x3 & value) const override;

    virtual void set(const Program * program, GLint location, const TextureHandle & value) const override;

    virtual void set(const Program * program, GLint location, const std::vector<float> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<int> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<unsigned int> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<bool> & value) const override;

    virtual void set(const Program * program, GLint location, const std::vector<glm::vec2> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::vec3> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::vec4> & value) const override;

    virtual void set(const Program * program, GLint location, const std::vector<glm::ivec2> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::ivec3> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::ivec4> & value) const override;

    virtual void set(const Program * program, GLint location, const std::vector<glm::uvec2> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::uvec3> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::uvec4> & value) const override;

    virtual void set(const Program * program, GLint location, const std::vector<glm::mat2> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::mat3> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::mat4> & value) const override;

    virtual void set(const Program * program, GLint location, const std::vector<glm::mat2x3> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::mat3x2> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::mat2x4> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::mat4x2> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::mat3x4> & value) const override;
    virtual void set(const Program * program, GLint location, const std::vector<glm::mat4x3> & value) const override;

    virtual void set(const Program * program, GLint location, const std::vector<TextureHandle> & value) const override;
};

} // namespace glow
