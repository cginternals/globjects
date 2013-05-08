#include <glow/Program.h>

#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace glow;

Program::Program()
: _linked(false)
, _dirty(true)
{
	_id = glCreateProgram();
}

Program::~Program()
{
	for (ref_ptr<Shader> shader: _shaders)
	{
		detach(shader);
	}
	if (_id) glDeleteProgram(_id);
}

void Program::use()
{
	glUseProgram(_id);
}

void Program::release()
{
	glUseProgram(0);
}

bool Program::isUsed() const
{
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	return currentProgram>0 && currentProgram == _id;
}

bool Program::isLinked() const
{
	return _linked;
}

void Program::invalidate()
{
	_dirty = true;
}

void Program::checkDirty()
{
	if (_dirty)
	{
		link();
	}
}

void Program::attach(Shader* shader)
{
	glAttachShader(_id, shader->id());

	shader->addToProgram(this);
	_shaders.insert(shader);

	invalidate();
}

void Program::detach(Shader* shader)
{
	glDetachShader(_id, shader->id());

	shader->removeFromProgram(this);
	_shaders.erase(shader);

	invalidate();
}

void Program::link()
{
	glLinkProgram(_id);
	checkLinkStatus();
	_dirty = false;
}

void Program::bindFragDataLocation(GLuint index, const std::string& name)
{
	glBindFragDataLocation(_id, index, name.c_str());
}

void Program::bindAttributeLocation(GLuint index, const std::string& name)
{
	glBindAttribLocation(_id, index, name.c_str());
}

GLint Program::getUniformLocation(const std::string& name)
{
	checkDirty();
	return glGetUniformLocation(_id, name.c_str());
}

GLint Program::getAttributeLocation(const std::string& name)
{
	checkDirty();
	return glGetAttribLocation(_id, name.c_str());
}

void Program::enableVertexAttribArray(GLint index)
{
	glEnableVertexAttribArray(index);
}

void Program::disableVertexAttribArray(GLint index)
{
	glDisableVertexAttribArray(index);
}

void Program::enableVertexAttribArray(const std::string& name)
{
	enableVertexAttribArray(getAttributeLocation(name));
}

void Program::disableVertexAttribArray(const std::string& name)
{
	disableVertexAttribArray(getAttributeLocation(name));
}

void Program::setUniform(const std::string& name, int value)
{
	use();
	glUniform1i(getUniformLocation(name), value);
	release();
}

void Program::setUniform(const std::string& name, float value)
{
	use();
	glUniform1f(getUniformLocation(name), value);
	release();
}

void Program::setUniform(const std::string& name, const glm::mat4& value)
{
	use();
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
        release();
}

std::string Program::infoLog() const
{
	GLsizei length;
	glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

	std::vector<char> log(length);

	glGetProgramInfoLog(_id, length, &length, log.data());

	return std::string(log.data(), length);
}

void Program::checkLinkStatus()
{
	GLint status = 0;
	glGetProgramiv(_id, GL_LINK_STATUS, &status);

	_linked = (status == GL_TRUE);

	if (!_linked)
	{
		std::cout
			<< "Linker error:" << std::endl
			<< infoLog() << std::endl;
	}
}
