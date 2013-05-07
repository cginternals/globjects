#include <glow/Program.h>

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace glow;

Program::Program()
{
	_id = glCreateProgram();
}

Program::~Program()
{
	for (Shader* shader: _shaders)
	{
		delete shader;
	}
	if (_id) glDeleteProgram(_id);
}

void Program::bind()
{
	glUseProgram(_id);
}

void Program::unbind()
{
	glUseProgram(0);
}

void Program::attach(Shader* shader)
{
	glAttachShader(_id, shader->id());
	_shaders.push_back(shader);
}

void Program::link()
{
	glLinkProgram(_id);
	checkLinkStatus();
}

void Program::bindFragDataLocation(GLuint index, const char* name)
{
	glBindFragDataLocation(_id, index, name);
}

void Program::bindAttribLocation(GLuint index, const char* name)
{
	glBindAttribLocation(_id, index, name);
}

GLint Program::getUniformLocation(const char* name)
{
	return glGetUniformLocation(_id, name);
}

GLint Program::getAttribLocation(const char* name)
{
	return glGetAttribLocation(_id, name);
}

void Program::enableVertexAttribArray(GLint index)
{
	glEnableVertexAttribArray(index);
}

void Program::disableVertexAttribArray(GLint index)
{
	glDisableVertexAttribArray(index);
}

void Program::enableVertexAttribArray(const char* name)
{
	enableVertexAttribArray(getAttribLocation(name));
}

void Program::disableVertexAttribArray(const char* name)
{
	disableVertexAttribArray(getAttribLocation(name));
}

void Program::setUniform(const char* name, int value)
{
	bind();
	glUniform1i(getUniformLocation(name), value);
	unbind();
}

void Program::setUniform(const char* name, float value)
{
	bind();
	glUniform1f(getUniformLocation(name), value);
	unbind();
}

void Program::setUniform(const char* name, const glm::mat4& value)
{
	bind();
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
        unbind();
}

void Program::checkLinkStatus()
{
	GLint status = 0;
	glGetProgramiv(_id, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLsizei length;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

		std::vector<char> log(length);

		glGetProgramInfoLog(_id, length, &length, log.data());

		std::cout << "Linker error:" << std::endl << log.data() << std::endl;
	}
}
