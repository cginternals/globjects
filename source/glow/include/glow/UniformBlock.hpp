#pragma once

#include <glow/UniformBlock.h>

namespace glow {

template <std::size_t Count>
std::array<GLint, Count> UniformBlock::getActive(GLenum pname)
{
    std::array<GLint, Count> result;
    getActive(pname, result.data());
    return result;
}

template <std::size_t Count>
std::array<GLint, Count> UniformBlock::getActiveUniforms(const std::array<GLuint, Count> & uniformIndices, GLenum pname)
{
    std::array<GLint, Count> result;
    getActiveUniforms(static_cast<GLint>(Count), uniformIndices.data(), pname, result.data());
    return result;
}

} // namespace glow
