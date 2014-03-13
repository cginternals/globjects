#pragma once

#include <GL/glew.h>

#include <array>
#include <vector>

#include <glow/glow.h>
#include <glow/LocationIdentity.h>

namespace glow {

class Program;

class GLOW_API UniformBlock
{
    friend class Program;
public:
    UniformBlock();
    UniformBlock(Program * program, const LocationIdentity & m_identity);

    const LocationIdentity & identity() const;

    void getActive(GLenum pname, GLint * params);
    template <std::size_t Count>
    std::array<GLint, Count> getActive(GLenum pname);

    void getActiveUniforms(GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params);
    std::vector<GLint> getActiveUniforms(const std::vector<GLuint> & uniformIndices, GLenum pname);
    template <std::size_t Count>
    std::array<GLint, Count> getActiveUniforms(const std::array<GLuint, Count> & uniformIndices, GLenum pname);

    void setBinding(GLuint bindingIndex);
protected:
    Program * m_program;
    LocationIdentity m_identity;
    GLuint m_bindingIndex;

    GLuint blockIndex();
    void updateBinding();
};

} // namespace glow

#include <glow/UniformBlock.hpp>
