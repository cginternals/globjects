
#pragma once

#include <GL/glew.h>

#include <string>
#include <unordered_map>
#include <set>

#include <glow/declspec.h>


namespace glow
{

class Program;

class GLOW_API Shader
{
public:
    typedef std::set<Program *> t_programs;

public:
    Shader(const GLenum type);
    virtual ~Shader();

    const bool setSource(
        const std::string & source
    ,   const bool update = true);
    const std::string & source() const;

    void update();

    const GLenum type() const;
    const GLuint shader() const;

    const bool isCompiled() const;

    const t_programs & programs() const;
    t_programs & programs();

protected:
    inline const bool isShader() const;

protected:
    const GLenum m_type;
    GLuint m_shader;

    std::string m_source;
    std::string m_log;
    
    bool m_compiled;

    t_programs m_programs;

protected:
    typedef std::unordered_map<GLenum, std::string> t_typeStrings;
    static const t_typeStrings typeStrings;

    static const t_typeStrings initializeTypeStrings();
};

} // namespace glow