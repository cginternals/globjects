#pragma once

#include <glow/glow.h>

#include <glbinding/types.h>
#include <glbinding/functions.h>

namespace glow 
{

template <int Count>
std::array<gl::GLint, Count> getIntegers(gl::GLenum pname)
{
    std::array<gl::GLint, Count> values;

    gl::GetIntegerv(pname, reinterpret_cast<gl::GLint*>(&values));

    return values;
}

template <int Count>
std::array<gl::GLfloat, Count> getFloats(gl::GLenum pname)
{
    std::array<gl::GLfloat, Count> values;

    gl::GetFloatv(pname, reinterpret_cast<gl::GLfloat*>(&values));

    return values;
}

template <int Count>
std::array<gl::GLdouble, Count> getDoubles(gl::GLenum pname)
{
    std::array<gl::GLdouble, Count> values;

    gl::GetDoublev(pname, reinterpret_cast<gl::GLdouble*>(&values));

    return values;
}

template <int Count>
std::array<gl::GLboolean, Count> getBooleans(gl::GLenum pname)
{
    std::array<gl::GLboolean, Count> values;

    gl::GetBooleanv(pname, reinterpret_cast<gl::GLboolean*>(&values));

    return values;
}

} // namespace glow
