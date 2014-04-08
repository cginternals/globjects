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
#include <glow/Buffer.h>

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

void Program::use() const
{
	checkDirty();

    if (!isLinked())
        return;

	glUseProgram(m_id);
	CheckGLError();
}

void Program::release() const
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

void Program::invalidate() const
{
	m_dirty = true;
}

void Program::notifyChanged(const Changeable *)
{
	invalidate();
}

void Program::checkDirty() const
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

void Program::link() const
{
    m_linked = false;

    if (!prepareForLinkage())
        return;

	glLinkProgram(m_id);
	CheckGLError();

    m_linked = checkLinkStatus();
	m_dirty = false;

    updateUniforms();
    updateUniformBlockBindings();
}

bool Program::prepareForLinkage() const
{
    if (m_binary && glow::hasExtension(GLOW_ARB_get_program_binary))
    {
        glProgramBinary(m_id, m_binary->format(), m_binary->data(), m_binary->length());
        CheckGLError();

        return true;
    }

    return compileAttachedShaders();
}

bool Program::compileAttachedShaders() const
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

bool Program::checkLinkStatus() const
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

void Program::bindFragDataLocation(GLuint index, const std::string & name) const
{
	glBindFragDataLocation(m_id, index, name.c_str());
	CheckGLError();
}

void Program::bindAttributeLocation(GLuint index, const std::string & name) const
{
	glBindAttribLocation(m_id, index, name.c_str());
	CheckGLError();
}

GLint Program::getUniformLocation(const std::string& name) const
{
	checkDirty();
    if (!m_linked)
        return -1;

	GLint result = glGetUniformLocation(m_id, name.c_str());
	CheckGLError();
	return result;
}

std::vector<GLint> Program::getAttributeLocations(const std::vector<std::string> & names) const
{
    std::vector<GLint> locations(names.size());
    for (unsigned i = 0; i<names.size(); ++i)
    {
        locations[i] = getAttributeLocation(names[i]);
    }
    return locations;
}

std::vector<GLint> Program::getUniformLocations(const std::vector<std::string> & names) const
{
    std::vector<GLint> locations(names.size());
    for (unsigned i = 0; i<names.size(); ++i)
    {
        locations[i] = getUniformLocation(names[i]);
    }
    return locations;
}

GLint Program::getAttributeLocation(const std::string & name) const
{
	checkDirty();
    if (!m_linked)
        return -1;

	GLint result = glGetAttribLocation(m_id, name.c_str());
	CheckGLError();
	return result;
}

GLuint Program::getResourceIndex(GLenum programInterface, const std::string & name) const
{
	checkDirty();

	GLuint result = glGetProgramResourceIndex(m_id, programInterface, name.c_str());
	CheckGLError();
	return result;
}

GLuint Program::getUniformBlockIndex(const std::string& name) const
{
    checkDirty();

    GLuint result = glGetUniformBlockIndex(m_id, name.c_str());
    CheckGLError();
    return result;
}

void Program::getActiveUniforms(GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params) const
{
    checkDirty();

    glGetActiveUniformsiv(m_id, uniformCount, uniformIndices, pname, params);
    CheckGLError();
}

std::vector<GLint> Program::getActiveUniforms(const std::vector<GLuint> & uniformIndices, GLenum pname) const
{
    std::vector<GLint> result(uniformIndices.size());
    getActiveUniforms(static_cast<GLint>(uniformIndices.size()), uniformIndices.data(), pname, result.data());
    return result;
}

std::vector<GLint> Program::getActiveUniforms(const std::vector<GLint> & uniformIndices, GLenum pname) const
{
    std::vector<GLuint> indices(uniformIndices.size());
    for (unsigned i=0; i<uniformIndices.size(); ++i)
        indices[i] = static_cast<GLuint>(uniformIndices[i]);
    return getActiveUniforms(indices, pname);
}

GLint Program::getActiveUniform(GLuint uniformIndex, GLenum pname) const
{
    GLint result = 0;
    getActiveUniforms(1, &uniformIndex, pname, &result);
    return result;
}

std::string Program::getActiveUniformName(GLuint uniformIndex) const
{
    checkDirty();

    GLint length = getActiveUniform(uniformIndex, GL_UNIFORM_NAME_LENGTH);
    std::vector<char> name(length);
    glGetActiveUniformName(m_id, uniformIndex, length, nullptr, name.data());
    CheckGLError();

    return std::string(name.data(), length);
}

UniformBlock * Program::uniformBlock(GLuint uniformBlockIndex)
{
    return getUniformBlockByIdentity(uniformBlockIndex);
}

UniformBlock * Program::uniformBlock(const std::string& name)
{
    return getUniformBlockByIdentity(name);
}

UniformBlock * Program::getUniformBlockByIdentity(const LocationIdentity & identity)
{
    checkDirty();

    if (m_uniformBlocks.find(identity) == m_uniformBlocks.end())
    {
        m_uniformBlocks[identity] = UniformBlock(this, identity);
    }

    return &m_uniformBlocks[identity];
}

void Program::addUniform(AbstractUniform * uniform)
{
    assert(uniform != nullptr);

    ref_ptr<AbstractUniform>& uniformReference = m_uniforms[uniform->identity()];

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

void Program::updateUniforms() const
{
	// Note: uniform update will check if program is linked
    for (std::pair<LocationIdentity, ref_ptr<AbstractUniform>> uniformPair : m_uniforms)
	{
		uniformPair.second->update(this);
	}
}

void Program::updateUniformBlockBindings() const
{
    for (std::pair<LocationIdentity, UniformBlock> pair : m_uniformBlocks)
    {
        pair.second.updateBinding();
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

void Program::setShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding) const
{
	checkDirty();
    if (!m_linked)
        return;

	glShaderStorageBlockBinding(m_id, storageBlockIndex, storageBlockBinding);
	CheckGLError();
}

} // namespace glow
