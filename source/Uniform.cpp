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
	delete _setter;
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

#ifdef GL_NV_bindless_texture
void Uniform::setHandle(GLuint64 value)
{
	Texture::Handle handle;
	handle.value = value;
	
	set(handle);
}

void Uniform::setHandles(const Array<GLuint64>& values)
{
	Array<Texture::Handle> handles;
	
	for (GLuint64 value : values)
	{
		Texture::Handle handle;
		handle.value = value;
		
		handles << handle;
	}
	
	set(handles);
}
#endif
