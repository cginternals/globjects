#include <glow/AbstractUniform.h>

#include <cassert>

#include <glow/Program.h>

namespace glow
{

AbstractUniform::AbstractUniform(GLint location)
: m_location(location)
, m_identity(location)
{
}

AbstractUniform::AbstractUniform(const std::string & name)
: m_name(name)
, m_location(-1)
, m_identity(name)
{
}

AbstractUniform::~AbstractUniform()
{
}

const std::string & AbstractUniform::name() const
{
	return m_name;
}

GLint AbstractUniform::location() const
{
    return m_location;
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

void AbstractUniform::update(Program * program)
{
    assert(program != nullptr);

	program->use();

    if (program->isLinked())
    {
        setValueAt(program->getUniformLocation(m_name));
    }
}

} // namespace glow
