
#pragma once


#include <globjects/Resource.h>


namespace globjects
{


template <typename Subclass>
template <typename... Args>
std::unique_ptr<Subclass> Instantiator<Subclass>::create(Args && ... args)
{
    return std::unique_ptr<Subclass>(new Subclass(std::forward<Args>(args)...));
}

template <typename Subclass>
template <typename... Args>
std::unique_ptr<Subclass> Instantiator<Subclass>::fromId(gl::GLuint id, Args && ... args)
{
    return create(std::unique_ptr<IDResource>(new ExternalResource(id)), std::forward<Args>(args)...);
}


} // namespace globjects
