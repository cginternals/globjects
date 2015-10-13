
#include "TextureImplementation_DirectStateAccessEXT.h"

#include <cassert>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "TextureImplementation_Legacy.h"

using namespace gl;

namespace globjects {

TextureImplementation_DirectStateAccessEXT::TextureImplementation_DirectStateAccessEXT()
{
}

TextureImplementation_DirectStateAccessEXT::~TextureImplementation_DirectStateAccessEXT()
{
}

gl::GLuint TextureImplementation_DirectStateAccessEXT::create(GLenum target) const
{
    return TextureImplementation_Legacy::instance()->create(target);
}

void TextureImplementation_DirectStateAccessEXT::destroy(gl::GLuint id) const
{
    TextureImplementation_Legacy::instance()->destroy(id);
}

} // namespace globjects
