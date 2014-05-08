#include "AbstractTransparencyAlgorithm.h"

#include <glow/Texture.h>

void AbstractTransparencyAlgorithm::initialize(const std::string & /*transparencyShaderFilePath*/, glow::Shader * /*vertexShader*/, glow::Shader * /*geometryShader*/)
{
}

void AbstractTransparencyAlgorithm::resize(int /*width*/, int /*height*/)
{
}

glow::Texture* AbstractTransparencyAlgorithm::createColorTex() {
    glow::Texture* color = new glow::Texture(gl::TEXTURE_2D);
    color->setParameter(gl::TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::LINEAR));
    color->setParameter(gl::TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::LINEAR));
    color->setParameter(gl::TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::REPEAT));
    color->setParameter(gl::TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::REPEAT));
    return color;
}
