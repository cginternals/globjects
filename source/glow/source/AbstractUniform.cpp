#include <glow/AbstractUniform.h>

#include <cassert>

#include <glow/Program.h>
#include <glow/Extension.h>

namespace glow
{

AbstractUniform::AbstractUniform(GLint location)
: m_identity(location)
{
}

AbstractUniform::AbstractUniform(const std::string & name)
: m_identity(name)
, m_directStateAccess(false)
, m_dsaCached(false)
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

    if (!m_dsaCached) // TODO: move caching to a per context caching
    {
        m_dsaCached = true;

        m_directStateAccess = hasExtension(GLOW_EXT_direct_state_access);
    }

    if (m_directStateAccess)
    {
		setValueAt(program, locationFor(program));
    }
    else
    {
        program->use();

        setValueAt(locationFor(program));
    }
}

} // namespace glow
