
 #pragma once


namespace globjects
{


template <std::size_t Count>
void TransformFeedback::setVaryings(const Program * program, const std::array<std::string, Count> & varyingNames, gl::GLenum bufferMode)
{
    std::array<const char*, Count> c_ptrs;

    for (size_t i = 0; i < Count; ++i)
    {
        c_ptrs[i] = varyingNames[i].c_str();
    }

    setVaryings(program, static_cast<gl::GLint>(Count), const_cast<const char**>(c_ptrs.data()), bufferMode);
}


} // namespace globjects
