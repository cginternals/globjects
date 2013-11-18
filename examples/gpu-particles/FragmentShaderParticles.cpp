
#include <GL/glew.h>

#include <glow/Program.h>
#include <glow/Shader.h>

#include <glowutils/Camera.h>
#include <glowutils/File.h>

#include "FragmentShaderParticles.h"


using namespace glow;
using namespace glm;


FragmentShaderParticles::FragmentShaderParticles(
    const Array<vec4> & positions
,   const Array<vec4> & velocities
,   const Texture & forces
,   const Camera & camera)
: AbstractParticleTechnique(positions, velocities, forces, camera)
{
}

FragmentShaderParticles::~FragmentShaderParticles()
{
}

void FragmentShaderParticles::initialize()
{
}

void FragmentShaderParticles::reset()
{

}

void FragmentShaderParticles::step(const float elapsed)
{
}

void FragmentShaderParticles::draw(const float elapsed)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    //m_drawProgram->use();
    //m_drawProgram->setUniform("viewProjection", m_camera.viewProjection());

    //m_drawProgram->release();

    //glDisable(GL_BLEND);
}

void FragmentShaderParticles::resize()
{
    //m_drawProgram->setUniform("aspect", m_camera.aspectRatio());
}
