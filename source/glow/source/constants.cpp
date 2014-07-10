#include <glow/constants.h>

#include <glbinding/Meta.h>

namespace glow {

std::string enumName(gl::GLenum param)
{
    return gl::Meta::getString(param);
}

} // namespace glow
