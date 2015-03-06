#include "AbstractParticleTechnique.h"

#include <cassert>

#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/VertexArray.h>

#include <globjects/base/File.h>

#include <common/ScreenAlignedQuad.h>
#include <common/Camera.h>


using namespace gl;
using namespace glm;
using namespace globjects;

AbstractParticleTechnique::AbstractParticleTechnique(
    const std::vector<vec4> & positions
,   const std::vector<vec4> & velocities
,   const Texture & forces
,   const Camera & camera)
: m_positions(positions)
, m_velocities(velocities)
, m_forces(forces)
, m_camera(camera)
, m_numParticles(static_cast<unsigned>(positions.size()))
, m_paused(false)
{
    assert(positions.size() == velocities.size());
}

AbstractParticleTechnique::~AbstractParticleTechnique()
{
}

void AbstractParticleTechnique::initialize(const std::string & vertexShaderSourceFilePath)
{
    m_fbo.reset(new Framebuffer());

    m_color.reset(new Texture(GL_TEXTURE_2D));
    m_color->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_color->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_color->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_color->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_color->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //m_color->image2D(0, GL_RGB16F, glm::ivec2(1, 1), 0, GL_RGB, GL_FLOAT, nullptr);
    m_color->image2D(0, GL_RGBA32F, glm::ivec2(1, 1), 0, GL_RGB, GL_FLOAT, nullptr);

    m_fbo->bind();
    m_fbo->attachTexture(GL_COLOR_ATTACHMENT0, m_color.get());
    m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
    m_fbo->unbind();

    m_clearSource.reset(new File("data/gpu-particles/clear.frag"));
    m_clearShader.reset(new Shader(GL_FRAGMENT_SHADER, m_clearSource.get()));

    m_clear.reset(new ScreenAlignedQuad(m_clearShader.get()));
    m_quad.reset(new ScreenAlignedQuad(m_color.get()));

    m_vertexSource.reset(new File(vertexShaderSourceFilePath));
    m_vertexShader.reset(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()));

    m_geometrySource.reset(new File("data/gpu-particles/points.geom"));
    m_geometryShader.reset(new Shader(GL_GEOMETRY_SHADER, m_geometrySource.get()));

    m_fragmentSource.reset(new File("data/gpu-particles/points.frag"));
    m_fragmentShader.reset(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()));

    m_drawProgram.reset(new Program());
    m_drawProgram->attach(m_vertexShader.get(), m_geometryShader.get(), m_fragmentShader.get());

    // try to provide a particle alpha that scales well for 10 as well as for 1000000 particles
    const float alpha = 1.0f - 0.9f * (glm::sqrt(glm::sqrt(static_cast<float>(m_numParticles))) / 30.f);


    m_alphaUniform.reset(new globjects::Uniform<float>("alpha", alpha));
    m_aspectRatioUniform.reset(new globjects::Uniform<float>("aspect", 1.0));
    m_elapsedUniform.reset(new globjects::Uniform<float>("elapsed", 0.0f));
    m_pausedUniform.reset(new globjects::Uniform<bool>("paused", m_paused));
    m_viewProjectionUniform.reset(new globjects::Uniform<glm::mat4>("viewProjection", m_camera.viewProjection()));

    m_drawProgram->attach(
        m_alphaUniform.get(),
        m_aspectRatioUniform.get(),
        m_viewProjectionUniform.get(),
        m_pausedUniform.get()
    );

    m_clear->program()->attach(
        m_elapsedUniform.get()
    );
}

void AbstractParticleTechnique::pause(bool paused)
{
    m_paused = paused;
    m_pausedUniform->set(paused);
}

void AbstractParticleTechnique::resize()
{
    m_aspectRatioUniform->set(m_camera.aspectRatio());

    //m_color->image2D(0, GL_RGB16F, m_camera.viewport(), 0, GL_RGB, GL_FLOAT, nullptr);
    m_color->image2D(0, GL_RGBA32F, m_camera.viewport(), 0, GL_RGB, GL_FLOAT, nullptr);

    m_fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_fbo->unbind();
}

void AbstractParticleTechnique::reset()
{
    if (!m_fbo)
        return;

    m_fbo->bind();
    m_fbo->printStatus(true);
    glClear(GL_COLOR_BUFFER_BIT);
    m_fbo->unbind();
}

void AbstractParticleTechnique::draw(const float elapsed)
{
    glDisable(GL_DEPTH_TEST);

    m_fbo->bind();

    glEnable(GL_BLEND);
    if (m_paused)
        // clear color buffer
        glClear(GL_COLOR_BUFFER_BIT);
    else
    {
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

        m_elapsedUniform->set(elapsed);
        m_clear->draw();
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw particles
    m_viewProjectionUniform->set(m_camera.viewProjection());

    draw_impl(); // uses m_drawProgram

    glDisable(GL_BLEND);

    m_fbo->unbind();

    m_quad->draw();

    glEnable(GL_DEPTH_TEST);

    glFinish();
}
