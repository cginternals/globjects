#include "AbstractTransparencyAlgorithm.h"

#include <globjects/Texture.h>

void AbstractTransparencyAlgorithm::initialize(const std::string & /*transparencyShaderFilePath*/, glo::Shader * /*vertexShader*/, glo::Shader * /*geometryShader*/)
{
}

void AbstractTransparencyAlgorithm::resize(int /*width*/, int /*height*/)
{
}

glo::Texture* AbstractTransparencyAlgorithm::createColorTex() {
    return glo::Texture::createDefault();
}
