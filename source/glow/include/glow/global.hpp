#pragma once

#include <glow/global.h>

#include <glow/Error.h>

namespace glow 
{

template <int Count>
std::array<GLint, Count> getIntegers(GLenum pname)
{
    std::array<GLint, Count> values;

    glGetIntegerv(pname, reinterpret_cast<GLint*>(&values));
    CheckGLError();

    return values;
}

template <int Count>
std::array<GLfloat, Count> getFloats(GLenum pname)
{
    std::array<GLfloat, Count> values;

    glGetFloatv(pname, reinterpret_cast<GLfloat*>(&values));
    CheckGLError();

    return values;
}

template <int Count>
std::array<GLdouble, Count> getDoubles(GLenum pname)
{
    std::array<GLdouble, Count> values;

    glGetDoublev(pname, reinterpret_cast<GLdouble*>(&values));
    CheckGLError();

    return values;
}

template <int Count>
std::array<GLboolean, Count> getBooleans(GLenum pname)
{
    std::array<GLboolean, Count> values;

    glGetBooleanv(pname, reinterpret_cast<GLboolean*>(&values));
    CheckGLError();

    return values;
}

} // namespace glow
