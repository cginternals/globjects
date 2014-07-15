#include <glow/constants.h>

#include <glbinding/Meta.h>

namespace glow {

std::string enumName(gl::GLenum param)
{
    return glbinding::Meta::getString(param);
}

} // namespace glow
