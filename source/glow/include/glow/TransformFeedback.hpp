#pragma once

#include <glow/TransformFeedback.h>

namespace glow {

template <std::size_t Count>
void TransformFeedback::setVaryings(const Program * program, const std::array<const char *, Count> &varyingNames, gl::GLenum bufferMode) const
{
    setVaryings(program, static_cast<gl::GLint>(Count), const_cast<const char**>(varyingNames.data()), bufferMode);
}

} // namespace glow
