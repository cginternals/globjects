#pragma once

#include <unordered_map>
#include <string>

#include <glow/ref_ptr.h>
#include <glow/AbstractUniform.h>
#include <glow/Program.h>
#include <glow/Uniform.h>

namespace glow {

class UniformGroup
{
public:
    UniformGroup();
    virtual ~UniformGroup();

    template <typename T>
    Uniform<T>* uniform(const std::string& name);
    template <typename T>
    const Uniform<T>* uniform(const std::string& name) const;
    void addUniform(AbstractUniform* uniform);

    void addToProgram(Program* program);
protected:
    std::unordered_map<std::string, glow::ref_ptr<AbstractUniform>> m_uniforms;
};

} // namespace glow

#include <glowutils/UniformGroup.hpp>
