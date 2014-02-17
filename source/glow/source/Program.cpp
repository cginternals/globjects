#include <glow/Program.h>

#include <cassert>
#include <vector>

#include <glow/logging.h>
#include <glow/global.h>
#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/ObjectVisitor.h>
#include <glow/Shader.h>
#include <glow/ProgramBinary.h>
#include <glow/Extension.h>

namespace glow
{

Program::Program()
: Object(createProgram())
, m_linked(false)
, m_dirty(true)
{
}

Program::Program(ProgramBinary * binary)
: Program()
{
    setBinary(binary);
}

Program::~Program()
{
	for (ref_ptr<Shader> shader: std::set<ref_ptr<Shader>>(m_shaders))
	{
		detach(shader);
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
    GLuint currentProgram = static_cast<GLuint>(getInteger(GL_CURRENT_PROGRAM));

    return currentProgram > 0 && currentProgram == m_id;
}

bool Program::isLinked() const
{
	return m_linked;
}

void Program::invalidate()
{
	m_dirty = true;
}

void Program::notifyChanged(Changeable *)
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
    m_linked = false;

    if (!prepareForLinkage())
        return;

	glLinkProgram(m_id);
	CheckGLError();

    m_linked = checkLinkStatus();
	m_dirty = false;

    updateUniforms();
}

bool Program::prepareForLinkage()
{
    if (m_binary && glow::hasExtension(GLOW_ARB_get_program_binary))
    {
        glProgramBinary(m_id, m_binary->format(), m_binary->data(), m_binary->length());
        CheckGLError();

        return true;
    }

    return compileAttachedShaders();
}

bool Program::compileAttachedShaders()
{
    for (Shader* shader : shaders())
    {
        if (!shader->isCompiled())
        {
            // Some drivers (e.g. nvidia-331 on Ubuntu 13.04 automatically compile shaders during program linkage)
            // but we don't want to depend on such behavior
            shader->compile();

            if (!shader->isCompiled())
            {
                return false;
            }
        }
    }

    return true;
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

void Program::setBinary(ProgramBinary * binary)
{
    if (m_binary == binary)
        return;

    if (m_binary)
        m_binary->deregisterListener(this);

    m_binary = binary;

    if (m_binary)
        m_binary->registerListener(this);
}

ProgramBinary * Program::getBinary() const
{
    if (!glow::hasExtension(GLOW_ARB_get_program_binary))
    {
        return nullptr;
    }

    int length = get(GL_PROGRAM_BINARY_LENGTH);

    if (length == 0)
    {
        return nullptr;
    }

    GLenum format;
    std::vector<char> binary(length);

    glGetProgramBinary(m_id, length, nullptr, &format, binary.data());
    CheckGLError();

    return new ProgramBinary(format, binary);
}

GLint Program::get(GLenum pname) const
{
    GLint value = 0;
	glGetProgramiv(m_id, pname, &value);
	CheckGLError();

	return value;
}

const std::string Program::infoLog() const
{
	GLint length = get(GL_INFO_LOG_LENGTH);

    if (length == 0)
    {
        return std::string();
    }

    std::vector<char> log(length);

	glGetProgramInfoLog(m_id, length, &length, log.data());
	CheckGLError();

	return std::string(log.data(), length);
}

void Program::dispatchCompute(const glm::uvec3 & numGroups)
{
    dispatchCompute(numGroups.x, numGroups.y, numGroups.z);
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

void Program::dispatchComputeGroupSize(const glm::uvec3 & numGroups, const glm::uvec3 & groupSizes)
{
    dispatchComputeGroupSize(numGroups.x, numGroups.y, numGroups.z, groupSizes.x, groupSizes.y, groupSizes.z);
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
