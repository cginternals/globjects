#include <globjects/constants.h>

#include <glbinding/Meta.h>

namespace globjects {

std::string enumName(gl::GLenum param)
{
    return glbinding::Meta::getString(param);
}

} // namespace globjects
