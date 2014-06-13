#pragma once

#include <glow/Program.h>

#include <cassert>

#include <glbinding/functions.h>

#include <glowbase/baselogging.h>

#include <glow/Uniform.h>
#include <glow/Shader.h>

namespace glow
{

template<typename T>
void Program::setUniformByIdentity(const LocationIdentity & identity, const T & value)
{
    Uniform<T> * uniform = getUniformByIdentity<T>(identity);
    if (!uniform)
    {
        warning() << "Uniform type mismatch on set uniform. Uniform will be replaced.";

        addUniform(identity.isName() ? new Uniform<T>(identity.name(), value) : new Uniform<T>(identity.location(), value));
        return;
    }
    uniform->set(value);
}

template<typename T>
Uniform<T> * Program::getUniformByIdentity(const LocationIdentity & identity)
{
    if (m_uniforms.count(identity))
        return m_uniforms.at(identity)->as<T>();

    // create new uniform if none named <name> exists

    Uniform<T> * uniform = identity.isName() ? new Uniform<T>(identity.name()) : new Uniform<T>(identity.location());

    m_uniforms[uniform->identity()] = uniform;
    uniform->registerProgram(this);

    return uniform;
}

template<typename T>
const Uniform<T> * Program::getUniformByIdentity(const LocationIdentity & identity) const
{
    if (m_uniforms.count(identity))
        return m_uniforms.at(identity)->as<T>();

    // create new uniform if none named <name> exists

    Uniform<T> * uniform = identity.isName() ? new Uniform<T>(identity.name()) : new Uniform<T>(identity.location());

    m_uniforms[uniform->identity()] = uniform;
    uniform->registerProgram(this);

    return uniform;
}


template<typename T>
void Program::setUniform(const std::string & name, const T & value)
{
    setUniformByIdentity(name, value);
}

template<typename T>
void Program::setUniform(gl::GLint location, const T & value)
{
    setUniformByIdentity(location, value);
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

template <class ...Shaders>
void Program::attach(Shader * shader, Shaders... shaders)
{
    assert(shader != nullptr);

    gl::AttachShader(m_id, shader->id());

    shader->registerListener(this);
    m_shaders.insert(shader);

    invalidate();

    attach(std::forward<Shaders>(shaders)...);
}

} // namespace glow
