#pragma once

#include <glow/global.h>

#include <glow/Error.h>

namespace glow 
{

template <int Count>
std::array<GLint, Count> query::getIntegers(GLenum pname)
{
    std::array<GLint, Count> values;

    glGetIntegerv(pname, reinterpret_cast<GLint*>(&values));
    CheckGLError();

    return values;
}

} // namespace glow
