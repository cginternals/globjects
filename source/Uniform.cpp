#include <glow/Uniform.h>
#include <glow/Program.h>


using namespace glow;

Uniform::Uniform(const std::string& name)
: _name(name)
, _setter(nullptr)
{
}

Uniform::~Uniform()
{
}

const std::string& Uniform::name() const
{
	return _name;
}

void Uniform::registerProgram(Program* program)
{
	_programs.insert(program);
}

void Uniform::deregisterProgram(Program* program)
{
	_programs.erase(program);
}

void Uniform::changed()
{
	for (Program* program: _programs)
	{
		setFor(program);
	}
}

void Uniform::setUniformSetter(internal::AbstractUniformSetter* setter)
{
	delete _setter;
	_setter = setter;
}

void Uniform::setFor(Program* program)
{
	if (_setter)
	{
		program->use();
		_setter->set(program->getUniformLocation(_name));
	}
}
