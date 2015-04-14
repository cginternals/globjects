#pragma once

#include <globjects/Program.h>

#include <cassert>

#include <globjects/base/baselogging.h>

#include <globjects/Uniform.h>

namespace globjects
{

template<typename T>
Uniform<T> * Program::getUniformByIdentity(const LocationIdentity & identity)
{
    if (m_uniforms.count(identity))
        return m_uniforms.at(identity)->as<T>();

    return nullptr;
}

template<typename T>
const Uniform<T> * Program::getUniformByIdentity(const LocationIdentity & identity) const
{
    if (m_uniforms.count(identity))
        return m_uniforms.at(identity)->as<T>();

    return nullptr;
}

template<typename T>
Uniform<T> * Program::getUniform(const std::string & name)
{
    return getUniformByIdentity<T>(name);
}

template<typename T>
const Uniform<T> * Program::getUniform(const std::string & name) const
{
    return getUniformByIdentity<T>(name);
}

template<typename T>
Uniform<T> * Program::getUniform(gl::GLint location)
{
    return getUniformByIdentity<T>(location);
}

template<typename T>
const Uniform<T> * Program::getUniform(gl::GLint location) const
{
    return getUniformByIdentity<T>(location);
}

template <class ...Resources>
void Program::attach(Shader * shader, Resources... resources)
{
    attach(shader);

    attach(std::forward<Resources>(resources)...);
}

template <class ...Resources>
void Program::attach(AbstractUniform * uniform, Resources... resources)
{
    attach(uniform);

    attach(std::forward<Resources>(resources)...);
}

} // namespace globjects
