#include <globjects/constants.h>

#include <glbinding/Meta.h>

namespace glo {

std::string enumName(gl::GLenum param)
{
    return glbinding::Meta::getString(param);
}

} // namespace glo
