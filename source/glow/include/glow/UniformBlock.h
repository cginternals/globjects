#pragma once

#include <GL/glew.h>

#include <array>
#include <vector>

#include <glow/glow.h>
#include <glow/LocationIdentity.h>

namespace glow {

class Program;
class Buffer;

class GLOW_API UniformBlock
{
    friend class Program;
public:
    UniformBlock();
    UniformBlock(const Program * program, const LocationIdentity & m_identity);

    const LocationIdentity & identity() const;

    void setBinding(GLuint bindingIndex);

    void getActive(GLenum pname, GLint * params) const;
    GLint getActive(GLenum pname) const;
    std::vector<GLint> getActive(GLenum pname, GLint paramCount) const;

    std::vector<GLint> getActiveUniformIndices() const;

    std::string getName() const;
protected:
    const Program * m_program;
    LocationIdentity m_identity;
    GLuint m_bindingIndex;

    GLuint blockIndex() const;
    void updateBinding() const;
};

} // namespace glow
