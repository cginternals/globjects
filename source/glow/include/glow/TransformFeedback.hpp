#pragma once

#include <glow/TransformFeedback.h>

namespace glow {

template <std::size_t Count>
void TransformFeedback::setVaryings(Program * program
,   const std::array<const char *, Count> &varyingNames
,   GLenum bufferMode)
{
    setVaryings(program, static_cast<GLint>(Count), const_cast<const char**>(varyingNames.data()), bufferMode);
}

} // namespace glow
