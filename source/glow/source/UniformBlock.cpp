#include <glow/UniformBlock.h>
#include <glow/Program.h>

namespace glow {

UniformBlock::UniformBlock()
: m_program(nullptr)
{
}

UniformBlock::UniformBlock(Program * program, const LocationIdentity & identity)
: m_program(program)
, m_identity(identity)
{
}

const LocationIdentity & UniformBlock::identity() const
{
    return m_identity;
}

void UniformBlock::setBinding(GLuint bindingIndex)
{
    m_bindingIndex = bindingIndex;

    updateBinding();
}

GLuint UniformBlock::blockIndex()
{
    if (m_identity.isLocation())
        return m_identity.location();

    if (m_identity.isName())
        return m_program->getUniformBlockIndex(m_identity.name());

    return GL_INVALID_INDEX;
}

void UniformBlock::updateBinding()
{
    if (!m_program->isLinked())
        return;

    glUniformBlockBinding(m_program->id(), blockIndex(), m_bindingIndex);
    CheckGLError();
}

void UniformBlock::getActive(GLenum pname, GLint * params)
{
    glGetActiveUniformBlockiv(m_program->id(), blockIndex(), pname, params);
    CheckGLError();
}

void UniformBlock::getActiveUniforms(GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params)
{
    glGetActiveUniformsiv(m_program->id(), uniformCount, uniformIndices, pname, params);
    CheckGLError();
}

std::vector<GLint> UniformBlock::getActiveUniforms(const std::vector<GLuint> & uniformIndices, GLenum pname)
{
    std::vector<GLint> result(uniformIndices.size());
    getActiveUniforms(static_cast<GLint>(uniformIndices.size()), uniformIndices.data(), pname, result.data());
    return result;
}

} // namespace glow
