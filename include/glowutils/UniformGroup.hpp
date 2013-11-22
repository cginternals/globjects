#pragma once

#include <glowutils/UniformGroup.h>

namespace glowutils 
{

template <typename T>
glow::Uniform<T> * UniformGroup::uniform(const std::string & name)
{
    if (m_uniforms.count(name))
        return m_uniforms[name]->as<T>();

    // create new uniform if none named <name> exists

    glow::Uniform<T> * uniform = new glow::Uniform<T>(name);

    m_uniforms[uniform->name()] = uniform;

    return uniform;
}

template <typename T>
const glow::Uniform<T> * UniformGroup::uniform(const std::string & name) const
{
    if (m_uniforms.count(name))
        return m_uniforms.at(name)->as<T>();

    return nullptr;
}

} // namespace glowutils
