
#pragma once


#include <vector>

#include <glm/glm.hpp>

#include <globjects/TextureHandle.h>
#include <globjects/AbstractUniform.h>


namespace globjects
{


class Program;


class AbstractUniformImplementation
{
public:
    AbstractUniformImplementation();
    virtual ~AbstractUniformImplementation();

    static AbstractUniformImplementation * get(AbstractUniform::BindlessImplementation impl =
        AbstractUniform::BindlessImplementation::SeparateShaderObjectsARB);

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

    virtual void set(const Program * program, gl::GLint location, const gl::GLuint64 & value) const = 0;

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

    virtual void set(const Program * program, gl::GLint location, const std::vector<gl::GLuint64> & value) const = 0;
    virtual void set(const Program * program, gl::GLint location, const std::vector<TextureHandle> & value) const;

#ifdef GLOBJECTS_USE_EIGEN

    virtual void set(const Program *program, gl::GLint location, const Eigen::Vector2f &value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Vector3f &value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Vector4f &value) const = 0;

    virtual void set(const Program *program, gl::GLint location, const Eigen::Vector2i &value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Vector3i &value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Vector4i &value) const = 0;

    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<uint, 2, 1> &value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<uint, 3, 1> &value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<uint, 4, 1> &value) const = 0;

    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix2f & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix3f & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix4f & value) const = 0;

    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<float, 2, 3> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<float, 3, 2> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<float, 2, 4> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<float, 4, 2> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<float, 3, 4> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const Eigen::Matrix<float, 4, 3> & value) const = 0;

    // Need to use Eigen aware vector
    virtual void set(const Program *program, gl::GLint location, const std::vector<Eigen::Vector2f> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const std::vector<Eigen::Vector3f> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const std::vector<Eigen::Vector4f> & value) const = 0;

    virtual void set(const Program *program, gl::GLint location, const std::vector<Eigen::Vector2i> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const std::vector<Eigen::Vector3i> & value) const = 0;
    virtual void set(const Program *program, gl::GLint location, const std::vector<Eigen::Vector4i> & value) const = 0;

#endif

};


} // namespace globjects
