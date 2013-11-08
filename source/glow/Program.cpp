#include <cassert>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <glow/logging.h>
#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/ObjectVisitor.h>
#include <glow/Shader.h>

#include <glow/Program.h>

namespace glow
{

Program::Program()
: Object(createProgram())
, m_linked(false)
, m_dirty(true)
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

	if (ownsGLObject())
	{
		glDeleteProgram(m_id);
		CheckGLError();
	}
}

GLuint Program::createProgram()
{
	GLuint result = glCreateProgram();
	CheckGLError();
	return result;
}

void Program::accept(ObjectVisitor& visitor)
{
	visitor.visitProgram(this);
}

void Program::use()
{
	checkDirty();

    if (!isLinked())
        return;

	glUseProgram(m_id);
	CheckGLError();
}

void Program::release()
{
    if (!isLinked())
        return;

    glUseProgram(0);
	CheckGLError();
}

bool Program::isUsed() const
{
	GLint currentProgram = 0;

	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	CheckGLError();

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

void Program::attach()
{
}

void Program::detach(Shader * shader)
{
    assert(shader != nullptr);

	glDetachShader(m_id, shader->id());
	CheckGLError();

	shader->deregisterListener(this);
	m_shaders.erase(shader);

	invalidate();
}

std::set<Shader*> Program::shaders() const
{
	std::set<Shader*> shaders;
	for (ref_ptr<Shader> shader: m_shaders)
		shaders.insert(shader);
	return shaders;
}

void Program::link()
{
	glLinkProgram(m_id);
	CheckGLError();

    m_linked = checkLinkStatus();
	m_dirty = false;

	updateUniforms();
	CheckGLError();
}

bool Program::checkLinkStatus()
{
    if (GL_FALSE == get(GL_LINK_STATUS))
    {
        critical()
            << "Linker error:" << std::endl
            << infoLog();
        return false;
    }
    return true;
}

void Program::bindFragDataLocation(GLuint index, const std::string& name)
{
	glBindFragDataLocation(m_id, index, name.c_str());
	CheckGLError();
}

void Program::bindAttributeLocation(GLuint index, const std::string& name)
{
	glBindAttribLocation(m_id, index, name.c_str());
	CheckGLError();
}

GLint Program::getUniformLocation(const std::string& name)
{
	checkDirty();
    if (!m_linked)
        return -1;

	GLint result = glGetUniformLocation(m_id, name.c_str());
	CheckGLError();
	return result;
}

GLint Program::getAttributeLocation(const std::string& name)
{
	checkDirty();
    if (!m_linked)
        return -1;

	GLint result = glGetAttribLocation(m_id, name.c_str());
	CheckGLError();
	return result;
}

GLuint Program::getResourceIndex(GLenum programInterface, const std::string& name)
{
	checkDirty();

	GLuint result = glGetProgramResourceIndex(m_id, programInterface, name.c_str());
	CheckGLError();
	return result;
}

void Program::addUniform(AbstractUniform * uniform)
{
    assert(uniform != nullptr);

	ref_ptr<AbstractUniform>& uniformReference = m_uniforms[uniform->name()];

	if (uniformReference)
	{
		uniformReference->deregisterProgram(this);
	}

	uniformReference = uniform;

	uniform->registerProgram(this);

	if (m_linked)
	{
		uniform->update(this);
	}
}

void Program::updateUniforms()
{
	// Note: uniform update will check if program is linked
	for (std::pair < std::string, ref_ptr<AbstractUniform>> uniformPair : m_uniforms)
	{
		uniformPair.second->update(this);
	}
}

GLint Program::get(GLenum pname) const
{
	GLint value;
	glGetProgramiv(m_id, pname, &value);
	CheckGLError();

	return value;
}

const std::string Program::infoLog() const
{
	GLint length = get(GL_INFO_LOG_LENGTH);

	std::vector<char> log(length);

	glGetProgramInfoLog(m_id, length, &length, log.data());
	CheckGLError();

	return std::string(log.data(), length);
}

void Program::dispatchCompute(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ)
{
	use();

    if (!m_linked)
        return;

	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
	CheckGLError();
}

void Program::dispatchComputeGroupSize(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ, GLuint groupSizeX, GLuint groupSizeY, GLuint groupSizeZ)
{
    use();

    if (!m_linked)
        return;

    glDispatchComputeGroupSizeARB(numGroupsX, numGroupsY, numGroupsZ, groupSizeX, groupSizeY, groupSizeZ);
    CheckGLError();
}

void Program::setShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding)
{
	checkDirty();
    if (!m_linked)
        return;

	glShaderStorageBlockBinding(m_id, storageBlockIndex, storageBlockBinding);
	CheckGLError();
}

} // namespace glow
