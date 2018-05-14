
#include "TextureStorageImplementation_Fallback.h"

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


TextureStorageImplementation_Fallback::TextureStorageImplementation_Fallback()
{
}

TextureStorageImplementation_Fallback::~TextureStorageImplementation_Fallback()
{
}

void TextureStorageImplementation_Fallback::storage1D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width) const
{
    const globjects::AbstractTextureImplementation & implementation = bindlessImplementation();

    for (int i = 0; i < levels; i++)
    {
        implementation.image1D(texture, i, internalFormat, width, 0, gl::GL_R, gl::GL_UNSIGNED_BYTE, nullptr);
        width = std::max(1, width / 2);
    }
}

void TextureStorageImplementation_Fallback::storage2D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    const globjects::AbstractTextureImplementation & implementation = bindlessImplementation();

    for (int i = 0; i < levels; i++)
    {
        implementation.image2D(texture, i, internalFormat, width, height, 0, gl::GL_R, gl::GL_UNSIGNED_BYTE, nullptr);
        width = std::max(1, width / 2);
        height = std::max(1, height / 2);
    }
}

void TextureStorageImplementation_Fallback::storage3D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth) const
{
    const globjects::AbstractTextureImplementation & implementation = bindlessImplementation();

    for (int i = 0; i < levels; i++)
    {
        implementation.image3D(texture, i, internalFormat, width, height, depth, 0, gl::GL_R, gl::GL_UNSIGNED_BYTE, nullptr);
        width = std::max(1, width / 2);
        height = std::max(1, height / 2);
        depth = std::max(1, depth / 2);
    }
}

void TextureStorageImplementation_Fallback::storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const
{
    bindlessImplementation().image2DMultisample(texture, samples, internalFormat, width, height, fixedSampleLocations);
}

void TextureStorageImplementation_Fallback::storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const
{
    bindlessImplementation().image3DMultisample(texture, samples, internalFormat, width, height, depth, fixedSampleLocations);
}

void TextureStorageImplementation_Fallback::cubeMapStorage(const Texture * texture, gl::GLint levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    storage2D(texture, levels, internalFormat, width, height);
}


} // namespace globjects
