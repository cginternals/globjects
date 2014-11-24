#pragma once

#include <globjects/TransformFeedback.h>

namespace globjects {

template <std::size_t Count>
void TransformFeedback::setVaryings(const Program * program, const std::array<std::string, Count> & varyingNames, gl::GLenum bufferMode)
{
    std::vector<const char*> c_ptrs;
    c_ptrs.reserve(varyingNames.size());

    for (auto & name : varyingNames)
    {
        c_ptrs.push_back(name.c_str());
    }

    setVaryings(program, static_cast<gl::GLint>(Count), const_cast<const char**>(c_ptrs.data()), bufferMode);
}

} // namespace globjects
