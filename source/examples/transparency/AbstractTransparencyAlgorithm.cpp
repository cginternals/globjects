#include "AbstractTransparencyAlgorithm.h"

#include <globjects/Texture.h>

void AbstractTransparencyAlgorithm::initialize(const std::string & /*transparencyShaderFilePath*/, glo::Shader * /*vertexShader*/, glo::Shader * /*geometryShader*/)
{
}

void AbstractTransparencyAlgorithm::resize(int /*width*/, int /*height*/)
{
}

glo::Texture* AbstractTransparencyAlgorithm::createColorTex() {
    glo::Texture* color = new glo::Texture(gl::GL_TEXTURE_2D);
    color->setParameter(gl::GL_TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));
    color->setParameter(gl::GL_TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));
    color->setParameter(gl::GL_TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::GL_REPEAT));
    color->setParameter(gl::GL_TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::GL_REPEAT));
    return color;
}
