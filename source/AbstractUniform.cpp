
#include <glow/Program.h>
#include <glow/AbstractUniform.h>


namespace glow
{

AbstractUniform::AbstractUniform(const std::string & name)
:	_name(name)
{
}

AbstractUniform::~AbstractUniform()
{
}

const std::string & AbstractUniform::name() const
{
	return _name;
}

void AbstractUniform::registerProgram(Program * program)
{
	_programs.insert(program);
}

void AbstractUniform::unregisterProgram(Program * program)
{
	_programs.erase(program);
}

void AbstractUniform::changed()
{
	for (Program * program : _programs)
		update(program);
}

} // namespace glow