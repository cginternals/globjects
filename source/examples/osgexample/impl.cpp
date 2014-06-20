#include "impl.h"

#include <glowbase/ref_ptr.h>
#include <glowbase/StaticStringSource.h>

#include <glow/glow.h>
#include <glow/logging.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/FrameBufferObject.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/StringTemplate.h>

#include <array>

using namespace glow;
using namespace gl;
using namespace glm;

const char * fragmentShaderSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D source;
uniform sampler2D source2;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
    fragColor = texture(source, v_uv) *texture(source2, v_uv);
}
)";

struct PrivateGlowInterface
{
    ivec2 size;
    ref_ptr<glowutils::ScreenAlignedQuad> quad;
    ref_ptr<Texture> texture;

    ref_ptr<Texture> osgTexture;
};

GlowInterface::GlowInterface()
: impl(new PrivateGlowInterface)
{
}

GlowInterface::~GlowInterface()
{
    delete impl;
}

void GlowInterface::initialize()
{
    if (!init())
    {
        fatal() << "error";
        return;
    }

    info() << versionString();

    impl->texture = Texture::createDefault();

    unsigned char data[4][4] = {
        { 255, 0, 255, 255 }, { 0, 0, 255, 255 },
        { 255, 255, 0, 255 }, { 255, 0, 0, 255 }
    };

    impl->texture->image2D(0, GL_RGB8, ivec2(2), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    impl->quad = new glowutils::ScreenAlignedQuad(Shader::fromString(GL_FRAGMENT_SHADER, fragmentShaderSource), impl->texture);
    impl->quad->setSamplerUniform(0);

    impl->quad->program()->setUniform("source2", 1);

    VertexArrayObject::unbind();
}

void GlowInterface::setupTexture(unsigned id, unsigned target)
{
    impl->osgTexture = Texture::fromId((GLuint)id, static_cast<GLenum>(target), false);

    impl->quad->setTexture(impl->osgTexture);
}

void GlowInterface::resize(int x, int y)
{
    impl->size = ivec2(x,y);

    //info() << "resize " << impl->size;
}

void GlowInterface::paint()
{
    FrameBufferObject::unbind();

    glViewport(0,0,impl->size.x, impl->size.y);

    impl->texture->bindActive(GL_TEXTURE1);

    impl->quad->draw();

    impl->texture->unbindActive(GL_TEXTURE1);

    VertexArrayObject::unbind();
}
