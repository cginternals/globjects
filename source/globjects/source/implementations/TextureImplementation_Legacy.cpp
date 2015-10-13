
#include "TextureImplementation_Legacy.h"

#include <glbinding/gl/functions.h>

#include <globjects/globjects.h>

namespace globjects {

TextureImplementation_Legacy::TextureImplementation_Legacy()
{
}

TextureImplementation_Legacy::~TextureImplementation_Legacy()
{
}

gl::GLuint TextureImplementation_Legacy::create(gl::GLenum target) const
{
    gl::GLuint result = 0;

    gl::glGenTextures(1, &result);
    gl::glBindTexture(target, result);

    return result;
}

void TextureImplementation_Legacy::destroy(gl::GLuint id) const
{
    gl::glDeleteTextures(1, &id);
}

} // namespace globjects
