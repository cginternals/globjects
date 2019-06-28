
#include <globjects/AbstractUniform.h>

#include <cassert>

#include <globjects/Program.h>
#include <globjects/globjects.h>

#include "registry/ImplementationRegistry.h"

#include "implementations/AbstractUniformImplementation.h"


using namespace gl;


namespace
{


const globjects::AbstractUniformImplementation & implementation()
{
    return globjects::ImplementationRegistry::current().uniformImplementation();
}


} // namespace


namespace globjects
{


void AbstractUniform::hintBindlessImplementation(BindlessImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}

AbstractUniform::AbstractUniform(const Program * program, const GLint location, const UniformType type)
: m_identity(location)
, m_program(program)
, m_type(type)
, m_location(-1)
{
}

AbstractUniform::AbstractUniform(const Program * program, const std::string & name, const UniformType type)
: m_identity(name)
, m_program(program)
, m_type(type)
, m_location(-1)
{
}

AbstractUniform::~AbstractUniform()
{
}

const std::string & AbstractUniform::name() const
{
    return m_identity.name();
}

GLint AbstractUniform::location() const
{
    return m_identity.location();
}

const LocationIdentity & AbstractUniform::identity() const
{
    return m_identity;
}

UniformType AbstractUniform::type() const
{
    return m_type;
}

void AbstractUniform::changed()
{
    update(false);
}

void AbstractUniform::update(bool invalidateLocation) const
{
    if (!m_program->isLinked())
    {
        m_location = -1;

        return;
    }

    if (invalidateLocation || m_location == -1)
    {
        m_location = m_identity.isName() ? m_program->getUniformLocation(m_identity.name()) : m_identity.location();
    }

    updateAt(m_location);
}

void AbstractUniform::setValue(const GLint location, const float & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const int & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const unsigned int & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const bool & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::vec2 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::vec3 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::vec4 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::ivec2 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::ivec3 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::ivec4 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::uvec2 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::uvec3 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::uvec4 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat2 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat3 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat4 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat2x3 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat3x2 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat2x4 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat4x2 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat3x4 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const glm::mat4x3 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const GLuint64 & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const TextureHandle & value) const
{
    setValue(location, value.handle());
}

void AbstractUniform::setValue(const GLint location, const std::vector<float> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<int> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<unsigned int> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<bool> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::vec2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::vec3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::vec4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::ivec2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::ivec3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::ivec4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::uvec2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::uvec3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::uvec4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat2x3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat3x2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat2x4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat4x2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat3x4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<glm::mat4x3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<GLuint64> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(const GLint location, const std::vector<TextureHandle> & value) const
{
    implementation().set(m_program, location, value);
}

#ifdef GLOBJECTS_USE_EIGEN

void AbstractUniform::setValue(gl::GLint location, const Eigen::Vector2f &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Vector3f &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Vector4f &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Vector2i &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Vector3i &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Vector4i &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<uint, 2, 1> &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<uint, 3, 1> &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<uint, 4, 1> &value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix2f & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix3f & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix4f & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<float, 2, 3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<float, 3, 2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<float, 2, 4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<float, 4, 2> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<float, 3, 4> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const Eigen::Matrix<float, 4, 3> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const std::vector<Eigen::Vector2f> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const std::vector<Eigen::Vector3f> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const std::vector<Eigen::Vector4f> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const std::vector<Eigen::Vector2i> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const std::vector<Eigen::Vector3i> & value) const
{
    implementation().set(m_program, location, value);
}

void AbstractUniform::setValue(gl::GLint location, const std::vector<Eigen::Vector4i> & value) const
{
    implementation().set(m_program, location, value);
}

#endif

} // namespace globjects
