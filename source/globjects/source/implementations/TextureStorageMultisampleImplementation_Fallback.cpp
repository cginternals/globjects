
#include "TextureStorageMultisampleImplementation_Fallback.h"

#include <cmath>

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "../registry/ImplementationRegistry.h"

#include "AbstractTextureImplementation.h"


namespace
{


const globjects::AbstractTextureImplementation & bindlessImplementation()
{
    return globjects::ImplementationRegistry::current().textureBindlessImplementation();
}


} // namespace


namespace globjects
{


TextureStorageMultisampleImplementation_Fallback::TextureStorageMultisampleImplementation_Fallback()
{
}

TextureStorageMultisampleImplementation_Fallback::~TextureStorageMultisampleImplementation_Fallback()
{
}

void TextureStorageMultisampleImplementation_Fallback::storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const
{
    bindlessImplementation().image2DMultisample(texture, samples, internalFormat, width, height, fixedSampleLocations);
}

void TextureStorageMultisampleImplementation_Fallback::storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const
{
    bindlessImplementation().image3DMultisample(texture, samples, internalFormat, width, height, depth, fixedSampleLocations);
}


} // namespace globjects
