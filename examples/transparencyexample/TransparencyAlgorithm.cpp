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

Texture* createColorTex() {
    Texture* color = new Texture(GL_TEXTURE_2D);
    color->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    color->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    color->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    color->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    color->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return color;
}

void GlBlendAlgorithm::initialize() {
    program = new Program();
    program->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/normal.frag"));
    program->attach(glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/normal.vert"));

    colorTex = createColorTex();
    fbo = new FrameBufferObject();
    fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, colorTex);
    depth = new RenderBufferObject();
    fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, depth);
    fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GlBlendAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    fbo->bind();

    glViewport(0, 0, width, height);
    camera->setViewport(width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckGLError();

    program->setUniform("viewprojectionmatrix", camera->viewProjection());
    program->setUniform("normalmatrix", camera->normal());
    program->setUniform("screenSize", glm::vec2(width, height));
    program->use();

    glEnable(GL_BLEND);
    CheckGLError();

    drawFunction(program.get());

    glDisable(GL_BLEND);
    CheckGLError();

    fbo->unbind();
}

void GlBlendAlgorithm::resize(int width, int height) {
    int result = FrameBufferObject::defaultFBO()->getAttachmentParameter(GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);

    colorTex->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    depth->storage(result == 16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT, width, height);
}

} // namespace glow
