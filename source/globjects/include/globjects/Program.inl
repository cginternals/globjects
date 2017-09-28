
#pragma once


#include <cassert>

#include <globjects/base/baselogging.h>

#include <globjects/Uniform.h>


namespace globjects
{


template<typename T>
void Program::setUniformByIdentity(const LocationIdentity & identity, const T & value)
{
    Uniform<T> * uniform = getUniformByIdentity<T>(identity);
    if (uniform)
    {
        uniform->set(value);
        return;
    }

    warning() << "Uniform type mismatch on set uniform. Uniform will be replaced.";

    addUniform(identity.isName() ? Uniform<T>::create(this, identity.name(), value) : Uniform<T>::create(this, identity.location(), value));
}

template<typename T>
Uniform<T> * Program::getUniformByIdentity(const LocationIdentity & identity)
{
    const auto it = m_uniforms.find(identity);

    if (it != m_uniforms.end())
    {
        return it->second->type() == UniformTypeHelper<T>::value
            ? static_cast<Uniform<T> *>(it->second.get())
            : nullptr;
    }

    // create new uniform if none named <name> exists
    auto uniform = identity.isName() ? Uniform<T>::create(this, identity.name()) : Uniform<T>::create(this, identity.location());
    auto uniformPtr = uniform.get();

    m_uniforms[uniform->identity()] = std::move(uniform);

    return uniformPtr;
}

template<typename T>
const Uniform<T> * Program::getUniformByIdentity(const LocationIdentity & identity) const
{
    const auto it = m_uniforms.find(identity);

    if (it != m_uniforms.end())
    {
        return it->second->type() == UniformTypeHelper<T>::value
            ? static_cast<Uniform<T> *>(it->second.get())
            : nullptr;
    }

    // create new uniform if none named <name> exists
    auto uniform = identity.isName() ? Uniform<T>::create(this, identity.name()) : Uniform<T>::create(this, identity.location());
    auto uniformPtr = uniform.get();

    m_uniforms[uniform->identity()] = std::move(uniform);

    return uniformPtr;
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
    attach(shader);

    attach(std::forward<Shaders>(shaders)...);
}

template <size_t Count>
std::array<gl::GLint, Count> Program::get(gl::GLenum pname) const
{
    std::array<gl::GLint, Count> values;

    glGetProgramiv(id(), pname, values.data());

    return values;
}


} // namespace globjects
