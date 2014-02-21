#include <glow/AbstractUniform.h>

#include <cassert>

#include <glow/Program.h>
#include <glow/Extension.h>

namespace glow
{

AbstractUniform::AbstractUniform(const std::string & name)
: m_name(name)
, m_directStateAccess(false)
, m_cacheDSA(false)
{
}

AbstractUniform::~AbstractUniform()
{
}

const std::string & AbstractUniform::name() const
{
	return m_name;
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

void AbstractUniform::update(Program * program)
{
    assert(program != nullptr);

    if (!m_cacheDSA) // TODO: move caching to a per context caching
    {
        m_cacheDSA = true;

        m_directStateAccess = hasExtension(GLOW_EXT_direct_state_access);
    }

    if (m_directStateAccess)
    {
        GLint location = program->getUniformLocation(m_name);

        if (location >= 0)
            setProgramLocation(program, location);
    }
    else
    {
        program->use();

        if (program->isLinked())
            setLocation(program->getUniformLocation(m_name));
    }
}

} // namespace glow
