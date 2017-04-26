
#pragma once


namespace globjects 
{


template <typename T, std::size_t Count>
void AbstractUniform::setValue(const gl::GLint location, const std::array<T, Count> & value) const
{
    setValue(location, std::vector<T>(value.data(), value.data()+Count));
}


} // namespace globjects
