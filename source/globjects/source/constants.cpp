#include <globjects/constants.h>

#include <glbinding/Meta.h>


using namespace gl;

namespace globjects 
{

std::string enumName(GLenum param)
{
    return glbinding::Meta::getString(param);
}

} // namespace globjects
