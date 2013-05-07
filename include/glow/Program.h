
#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <set>
#include <string>

#include <glow/declspec.h>


namespace glow
{
class Shader;

class GLOW_API Program
{
public:
    typedef QSet<Shader *> t_shaders;

public:
    Program();
    virtual ~Program();

    const bool use() const;
    const bool release() const;

    const GLuint program() const;

    const bool attach(Shader * shader);
    const bool detach(Shader * shader);

    const t_shaders & shaders();

    const bool link() const;
    const bool isLinked() const;
    const bool isUsed() const;

    void invalidate();

    // location access

    // TODO: location cache

    const GLint attributeLocation(const std::string & name) const;
    const GLint uniformLocation(const std::string & name) const;

    // TODO: value access

protected:
    inline const bool isProgram() const;

protected:
    GLuint m_program;

    mutable std::string m_log;

	mutable bool m_linked;
    mutable bool m_dirty;

    t_shaders m_shaders;
};

} // namespace glow