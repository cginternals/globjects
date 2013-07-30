
#include <glow/Program.h>
#include <glow/AbstractUniform.h>


namespace glow
{

AbstractUniform::AbstractUniform(const std::string & name)
:	m_name(name)
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
	m_programs.insert(program);
}

void AbstractUniform::deregisterProgram(Program * program)
{
	m_programs.erase(program);
}

void AbstractUniform::changed()
{
	for (Program * program : m_programs)
		update(program);
}

} // namespace glow
