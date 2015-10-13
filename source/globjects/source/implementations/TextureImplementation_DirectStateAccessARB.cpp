
#include "TextureImplementation_DirectStateAccessARB.h"

#include <cassert>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "TextureImplementation_Legacy.h"

using namespace gl;

namespace globjects {

TextureImplementation_DirectStateAccessARB::TextureImplementation_DirectStateAccessARB()
{
}

TextureImplementation_DirectStateAccessARB::~TextureImplementation_DirectStateAccessARB()
{
}

gl::GLuint TextureImplementation_DirectStateAccessARB::create(gl::GLenum target) const
{
    gl::GLuint result = 0;

    gl::glCreateTextures(target, 1, &result);

    return result;
}

void TextureImplementation_DirectStateAccessARB::destroy(gl::GLuint id) const
{
    TextureImplementation_Legacy::instance()->destroy(id);
}

} // namespace globjects
