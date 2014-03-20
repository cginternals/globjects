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
    UniformBlock(Program * program, const LocationIdentity & m_identity);

    const LocationIdentity & identity() const;

    void setBinding(GLuint bindingIndex);

    void getActive(GLenum pname, GLint * params);
    GLint getActive(GLenum pname);
    std::vector<GLint> getActive(GLenum pname, GLint paramCount);

    std::vector<GLint> getActiveUniformIndices();

    std::string getName();
protected:
    Program * m_program;
    LocationIdentity m_identity;
    GLuint m_bindingIndex;

    GLuint blockIndex();
    void updateBinding();
};

} // namespace glow
