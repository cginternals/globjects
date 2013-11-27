#include "TransparencyAlgorithm.h"

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/DebugMessageOutput.h>
#include <glow/Buffer.h>

#include <glowutils/File.h>
#include <glowutils/Camera.h>

namespace glow {

Texture* TransparencyAlgorithm::createColorTex() {
    Texture* color = new Texture(GL_TEXTURE_2D);
    color->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    color->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    color->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    color->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    color->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return color;
}

} // namespace glow
