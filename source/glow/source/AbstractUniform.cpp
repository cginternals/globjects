#include <glow/AbstractUniform.h>

#include <cassert>

#include <glow/Program.h>
#include <glow/global.h>
#include <glow/Registry.h>
#include <glow/BehaviorRegistry.h>

#include "behaviors/AbstractUniformBehavior.h"

namespace glow
{

AbstractUniform::AbstractUniform(GLint location)
: m_identity(location)
{
}

AbstractUniform::AbstractUniform(const std::string & name)
: m_identity(name)
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

void AbstractUniform::registerProgram(Program * program)
{
    assert(program != nullptr);

	m_programs.insert(program);
}

void AbstractUniform::deregisterProgram(Program * program)
{
    assert(program != nullptr);

	m_programs.erase(program);
}

void AbstractUniform::changed()
{
	for (Program * program : m_programs)
		update(program);
}

GLint AbstractUniform::locationFor(const Program *program) const
{
    if (m_identity.isLocation())
        return m_identity.location();

    return program->getUniformLocation(m_identity.name());
}

void AbstractUniform::update(const Program * program) const
{
    assert(program != nullptr);

    if (!program->isLinked())
    {
        return;
    }

    updateAt(program, locationFor(program));
}

const AbstractUniformBehavior & AbstractUniform::behavior() const
{
    return glow::Registry::current().behaviors().uniformBehavior();
}

void AbstractUniform::setValue(const Program * program, GLint location, const float & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const int & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const unsigned int & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const bool & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::vec2 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::vec3 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::vec4 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::ivec2 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::ivec3 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::ivec4 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::uvec2 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::uvec3 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::uvec4 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat2 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat3 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat4 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat2x3 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat3x2 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat2x4 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat4x2 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat3x4 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const glm::mat4x3 & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const TextureHandle & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<float> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<int> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<unsigned int> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<bool> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::vec2> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::vec3> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::vec4> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::ivec2> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::ivec3> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::ivec4> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::uvec2> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::uvec3> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::uvec4> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat2> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat3> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat4> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat2x3> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat3x2> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat2x4> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat4x2> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat3x4> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<glm::mat4x3> & value) const
{
    behavior().set(program, location, value);
}

void AbstractUniform::setValue(const Program * program, GLint location, const std::vector<TextureHandle> & value) const
{
    behavior().set(program, location, value);
}

} // namespace glow
