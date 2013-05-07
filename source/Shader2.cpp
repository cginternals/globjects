#include <glow/Shader.h>

#include <vector>
#include <iostream>

using namespace glow;

Shader::Shader(GLenum type)
: _type(type)
{
	_id = glCreateShader(type);
}

Shader::~Shader()
{
	if (_id) glDeleteShader(_id);
}

GLenum Shader::type() const
{
	return _type;
}

void Shader::source(const char* source)
{
	glShaderSource(_id, 1, &source, 0);
}

void Shader::compile()
{
	glCompileShader(_id);
	checkCompileStatus();
}

void Shader::checkCompileStatus()
{
	GLint status = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLsizei length;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);

		std::vector<char> log(length);

		glGetShaderInfoLog(_id, length, &length, log.data());

		std::cout << "Compiler error:" << std::endl << log.data() << std::endl;
	}
}
