#pragma once

#include <glowutils/UniformGroup.h>

namespace glow 
{

template <typename T>
Uniform<T> * UniformGroup::uniform(const std::string & name)
{
    if (m_uniforms.count(name))
        return m_uniforms[name]->as<T>();

    // create new uniform if none named <name> exists

    Uniform<T> * uniform = new Uniform<T>(name);

    m_uniforms[uniform->name()] = uniform;

    return uniform;
}

template <typename T>
const Uniform<T> * UniformGroup::uniform(const std::string & name) const
{
    if (m_uniforms.count(name))
        return m_uniforms.at(name)->as<T>();

    return nullptr;
}

} // namespace glow
