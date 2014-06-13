#pragma once

#include <vector>

#include <glow/glow_api.h>
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

    void setBinding(gl::GLuint bindingIndex);

    void getActive(gl::GLenum pname, gl::GLint * params) const;
    gl::GLint getActive(gl::GLenum pname) const;
    std::vector<gl::GLint> getActive(gl::GLenum pname, gl::GLint paramCount) const;

    std::vector<gl::GLint> getActiveUniformIndices() const;

    std::string getName() const;
protected:
    const Program * m_program;
    LocationIdentity m_identity;
    gl::GLuint m_bindingIndex;

    gl::GLuint blockIndex() const;
    void updateBinding() const;
};

} // namespace glow
