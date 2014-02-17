#include <glow/AbstractUniform.h>

#include <cassert>

#include <glow/Program.h>

namespace glow
{

AbstractUniform::AbstractUniform(const std::string & name)
: m_name(name)
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

	program->use();

    if (program->isLinked())
	    setLocation(program->getUniformLocation(m_name));
}

} // namespace glow
