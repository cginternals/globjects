#include "AbstractTransparencyAlgorithm.h"

#include <glow/Texture.h>

void AbstractTransparencyAlgorithm::initialize(const std::string & /*transparencyShaderFilePath*/, glow::Shader * /*vertexShader*/, glow::Shader * /*geometryShader*/)
{
}

void AbstractTransparencyAlgorithm::resize(int /*width*/, int /*height*/)
{
}

glow::Texture* AbstractTransparencyAlgorithm::createColorTex() {
    glow::Texture* color = new glow::Texture(gl::GL_TEXTURE_2D);
    color->setParameter(gl::GL_TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));
    color->setParameter(gl::GL_TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));
    color->setParameter(gl::GL_TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::GL_REPEAT));
    color->setParameter(gl::GL_TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::GL_REPEAT));
    return color;
}
