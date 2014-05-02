#include "AbstractTransparencyAlgorithm.h"

#include <glow/Texture.h>

void AbstractTransparencyAlgorithm::initialize(const std::string & /*transparencyShaderFilePath*/, glow::Shader * /*vertexShader*/, glow::Shader * /*geometryShader*/)
{
}

void AbstractTransparencyAlgorithm::resize(int /*width*/, int /*height*/)
{
}

glow::Texture* AbstractTransparencyAlgorithm::createColorTex() {
    glow::Texture* color = new glow::Texture(GL_TEXTURE_2D);
    color->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    color->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    color->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    color->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    return color;
}
