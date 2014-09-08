#include "impl.h"

#include <glbinding/gl/gl.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/StaticStringSource.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Framebuffer.h>

#include <common/ScreenAlignedQuad.h>
#include <common/StringTemplate.h>

#include <array>

using namespace gl;
using namespace glm;
using namespace glo;

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

struct PrivateGlobjectsInterface
{
    ivec2 size;
    ref_ptr<ScreenAlignedQuad> quad;
    ref_ptr<Texture> texture;

    ref_ptr<Texture> osgTexture;
};

GlobjectsInterface::GlobjectsInterface()
: impl(new PrivateGlobjectsInterface)
{
}

GlobjectsInterface::~GlobjectsInterface()
{
    delete impl;
}

void GlobjectsInterface::initialize()
{
    glo::init();

    info() << versionString();

    impl->texture = Texture::createDefault();

    unsigned char data[4][4] = {
        { 255, 0, 255, 255 }, { 0, 0, 255, 255 },
        { 255, 255, 0, 255 }, { 255, 0, 0, 255 }
    };

    impl->texture->image2D(0, GL_RGB8, ivec2(2), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    impl->quad = new ScreenAlignedQuad(Shader::fromString(GL_FRAGMENT_SHADER, fragmentShaderSource), impl->texture);
    impl->quad->setSamplerUniform(0);

    impl->quad->program()->setUniform("source2", 1);

    VertexArray::unbind();
}

void GlobjectsInterface::setupTexture(unsigned id, unsigned target)
{
    impl->osgTexture = Texture::fromId(static_cast<GLuint>(id), static_cast<GLenum>(target));

    impl->quad->setTexture(impl->osgTexture);
}

void GlobjectsInterface::resize(int x, int y)
{
    impl->size = ivec2(x,y);

    //info() << "resize " << impl->size;
}

void GlobjectsInterface::paint()
{
//    Framebuffer::unbind();

    glViewport(0,0,impl->size.x, impl->size.y);

    impl->texture->bindActive(GL_TEXTURE1);

    impl->quad->draw();

    impl->texture->unbindActive(GL_TEXTURE1);

    VertexArray::unbind();
}
