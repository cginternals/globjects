#include <glow/UniformBlock.h>
#include <glow/Program.h>
#include <glow/Buffer.h>

namespace glow {

UniformBlock::UniformBlock()
: m_program(nullptr)
, m_bindingIndex(0)
{
}

UniformBlock::UniformBlock(const Program * program, const LocationIdentity & identity)
: m_program(program)
, m_identity(identity)
, m_bindingIndex(0)
{
}

const LocationIdentity & UniformBlock::identity() const
{
    return m_identity;
}

void UniformBlock::setBinding(gl::GLuint bindingIndex)
{
    m_bindingIndex = bindingIndex;

    updateBinding();
}

gl::GLuint UniformBlock::blockIndex() const
{
    if (m_identity.isLocation())
        return m_identity.location();

    if (m_identity.isName())
        return m_program->getUniformBlockIndex(m_identity.name());

    return gl::INVALID_INDEX;
}

void UniformBlock::updateBinding() const
{
    m_program->checkDirty();

    gl::UniformBlockBinding(m_program->id(), blockIndex(), m_bindingIndex);
}

void UniformBlock::getActive(gl::GLenum pname, gl::GLint * params) const
{
    m_program->checkDirty();

    gl::GetActiveUniformBlockiv(m_program->id(), blockIndex(), pname, params);
}

gl::GLint UniformBlock::getActive(gl::GLenum pname) const
{
    gl::GLint result = 0;
    getActive(pname, &result);
    return result;
}

std::vector<gl::GLint> UniformBlock::getActive(gl::GLenum pname, gl::GLint paramCount) const
{
    std::vector<gl::GLint> result(paramCount);
    getActive(pname, result.data());
    return result;
}

std::vector<gl::GLint> UniformBlock::getActiveUniformIndices() const
{
    return getActive(gl::UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, getActive(gl::UNIFORM_BLOCK_ACTIVE_UNIFORMS));}

std::string UniformBlock::getName() const
{
    if (m_identity.isName())
        return m_identity.name();

    gl::GLint length = getActive(gl::UNIFORM_BLOCK_NAME_LENGTH);
    std::vector<char> name(length);

    gl::GetActiveUniformBlockName(m_program->id(), blockIndex(), length, nullptr, name.data());

    return std::string(name.data(), length);
}

} // namespace glow
