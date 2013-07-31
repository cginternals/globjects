
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <glow/Program.h>
#include <glow/Log.h>

namespace glow
{

Program::Program()
:	Object(createProgram())
,	m_linked(false)
,	m_dirty(true)
{
}

Program::~Program()
{
	for (ref_ptr<Shader> shader: std::set<ref_ptr<Shader>>(m_shaders))
	{
		detach(shader);
	}
	for (std::pair<std::string, ref_ptr<AbstractUniform>> uniformPair: m_uniforms)
	{
		uniformPair.second->deregisterProgram(this);
	}
	if (m_id) 
		glDeleteProgram(m_id);
}

GLuint Program::createProgram()
{
	return glCreateProgram();
}

void Program::use()
{
	checkDirty();
	glUseProgram(m_id);
}

void Program::release()
{
	glUseProgram(0);
}

bool Program::isUsed() const
{
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	return currentProgram > 0 && currentProgram == (int)m_id;
}

bool Program::isLinked() const
{
	return m_linked;
}

void Program::invalidate()
{
	m_dirty = true;
}

void Program::notifyChanged()
{
	invalidate();
}

void Program::checkDirty()
{
	if (m_dirty)
	{
		link();
	}
}

void Program::attach(Shader* shader)
{
	glAttachShader(m_id, shader->id());

	shader->registerListener(this);
	m_shaders.insert(shader);

	invalidate();
}

void Program::detach(Shader* shader)
{
	glDetachShader(m_id, shader->id());

	shader->deregisterListener(this);
	m_shaders.erase(shader);

	invalidate();
}

void Program::link()
{
	glLinkProgram(m_id);
	checkLinkStatus();
	m_dirty = false;
	updateUniforms();
}

void Program::bindFragDataLocation(GLuint index, const std::string& name)
{
	glBindFragDataLocation(m_id, index, name.c_str());
}

void Program::bindAttributeLocation(GLuint index, const std::string& name)
{
	glBindAttribLocation(m_id, index, name.c_str());
}

GLint Program::getUniformLocation(const std::string& name)
{
	checkDirty();
	return glGetUniformLocation(m_id, name.c_str());
}

GLint Program::getAttributeLocation(const std::string& name)
{
	checkDirty();
	return glGetAttribLocation(m_id, name.c_str());
}

GLuint Program::getResourceIndex(GLenum programInterface, const std::string& name)
{
	checkDirty();
	return glGetProgramResourceIndex(m_id, programInterface, name.c_str());
}

void Program::addUniform(AbstractUniform * uniform)
{
	ref_ptr<AbstractUniform> & u(m_uniforms[uniform->name()]);

	if (u)
		u->deregisterProgram(this);

	u = uniform;
	uniform->registerProgram(this);

	if (m_linked) 
		uniform->update(this);
}

void Program::updateUniforms()
{
	for (std::pair < std::string, ref_ptr<AbstractUniform>> uniformPair : m_uniforms)
		uniformPair.second->update(this);
}

const std::string Program::infoLog() const
{
	GLsizei length;
	glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

	std::vector<char> log(length);

	glGetProgramInfoLog(m_id, length, &length, log.data());

	return std::string(log.data(), length);
}

void Program::checkLinkStatus()
{
	GLint status = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &status);

	m_linked = (status == GL_TRUE);

	if (!m_linked)
	{
		error()
			<< "Linker error:" << std::endl
			<< infoLog();
	}
}

void Program::dispatchCompute(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ)
{
	use();
	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}

void Program::setShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding)
{
	checkDirty();
	glShaderStorageBlockBinding(m_id, storageBlockIndex, storageBlockBinding);
}

void Program::attach(Shader* shader1, Shader* shader2, Shader* shader3, Shader* shader4, Shader* shader5)
{
	if (shader1) 
		attach(shader1);
	if (shader2) 
		attach(shader2);
	if (shader3) 
		attach(shader3);
	if (shader4) 
		attach(shader4);
	if (shader5) 
		attach(shader5);
}

} // namespace glow
