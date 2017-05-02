
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>


namespace globjects
{


/**
*  @brief
*    CRTP for creating objects similar to std::make_unique
*/
template <typename Subclass>
class GLOBJECTS_TEMPLATE_API Instantiator
{
public:
    template <typename... Args>
    static std::unique_ptr<Subclass> create(Args && ... args);

    template <typename... Args>
    static std::unique_ptr<Subclass> fromId(gl::GLuint id, Args && ... args);
};


} // namespace globjects


#include <globjects/base/Instantiator.inl>
