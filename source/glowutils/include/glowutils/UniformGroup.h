#pragma once

#include <unordered_map>
#include <string>

#include <glow/AbstractUniform.h>
#include <glow/Uniform.h>

#include <glowutils/glowutils_api.h>

namespace glow
{
    class AbstractUniform;
    class Program;
}

namespace glowutils
{

class GLOWUTILS_API UniformGroup
{
public:
    UniformGroup();
    virtual ~UniformGroup();

    template <typename T>
    glow::Uniform<T> * uniform(const std::string & name);

    template <typename T>
    const glow::Uniform<T> * uniform(const std::string & name) const;

    void addUniform(glow::AbstractUniform * uniform);
    void addToProgram(glow::Program * program);

protected:
    std::unordered_map<std::string, glow::ref_ptr<glow::AbstractUniform>> m_uniforms;
};

} // namespace glowutils

#include <glowutils/UniformGroup.hpp>
