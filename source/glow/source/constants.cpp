#include <glow/constants.h>

#include <glbinding/meta.h>

namespace glow {

std::string enumName(gl::GLenum param)
{
    return gl::meta::getName(param);
}

} // namespace glow
