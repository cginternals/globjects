
#pragma once


namespace globjects 
{


template <typename T, std::size_t Count>
void AbstractUniform::setValue(const Program * program, const gl::GLint location, const std::array<T, Count> & value) const
{
    setValue(program, location, std::vector<T>(value.data(), value.data()+Count));
}


} // namespace globjects
