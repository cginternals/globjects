
#include <GL/glew.h>

#include <algorithm>
#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/Timer.h>
#include <glow/Texture.h>
#include <glow/Array.h>

#include <glowutils/Camera.h>
#include <glowutils/File.h>
#include <glowutils/FileRegistry.h>
#include <glowutils/MathMacros.h>

#include <glowwindow/Context.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include "AbstractParticleTechnique.h"

#include "ComputeShaderParticles.h"
#include "FragmentShaderParticles.h"
#include "TransformFeedbackParticles.h"

using namespace glow;
using namespace glm;


class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    : m_technique(ComputeShaderTechnique)
    , m_numParticles(1000)
    , m_camera(nullptr)
    {
        m_timer.start();

        // Initialize Particle Positions and Attributes

        m_positions.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_positions[i] = vec4(randf(-1.f, +1.f), randf(-1.f, +1.f), randf(-1.f, +1.f), 0.f);

        m_velocities.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_velocities[i] = vec4(0.f);

        m_attributes.resize(m_numParticles);
        Attribute attribute;
        for (int i = 0; i < m_numParticles; ++i)
        {
            // ToDo:
            attribute.moep = 0;

            m_attributes[i] = attribute;
        }
    }

    virtual ~EventHandler()
    {
        delete m_camera;
    }

    virtual void initialize(Window & window) override
    {
        DebugMessageOutput::enable();


        // initialize 3D Force Field (3D Texture)

        static const ivec3 fdim(16, 16, 16); //  // 4k * 3

        Array<vec3> forces;
        forces.resize(fdim.x * fdim.y * fdim.z);

        for (int z = 0; z < fdim.z; ++z)
        for (int y = 0; y < fdim.y; ++y)
        for (int x = 0; x < fdim.x; ++x)
        {
            const int i = z *  fdim.x * fdim.y + y * fdim.x + x;
            const vec3 f(randf(-1.0, +1.f), randf(-1.0, +1.f), randf(-1.0, +1.f));

            forces[i] = f;
        }
        
        m_forces = new Texture(GL_TEXTURE_3D);

        m_forces->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_forces->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        m_forces->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        m_forces->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_forces->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        m_forces->image3D(0, GL_RGB32F, fdim.x, fdim.y, fdim.z, 0, GL_RGB, GL_FLOAT, forces.data());

        
        // initialize camera

        m_camera = new Camera(vec3(0.f, 0.f, -4.f));

        
        // initialize techniques

        m_techniques[ComputeShaderTechnique] = new ComputeShaderParticles(
            m_positions, m_velocities, *m_forces, *m_camera);
        m_techniques[FragmentShaderTechnique] = new FragmentShaderParticles(
            m_positions, m_velocities, *m_forces, *m_camera);
        m_techniques[TransformFeedbackTechnique] = new TransformFeedbackParticles(
            m_positions, m_velocities, *m_forces, *m_camera);

        for (auto technique : m_techniques)
            technique.second->initialize();

        reset();
    }
    
    virtual void resizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        m_camera->setViewport(event.size());

        for (auto technique : m_techniques)
            technique.second->resize();
    }

    virtual void idle(Window & window) override
    {
        float f = static_cast<float>(m_timer.elapsed() * 1e-10);
        m_camera->setEye(vec3(cos(f), 0.f, sin(f)) * 4.f);

        step();
        window.repaint();
    }

    virtual void paintEvent(PaintEvent &) override
    {
        draw();
    }

    void step()
    {
        static const int steps = 1;
        static const float stepsinv = 1.f / static_cast<float>(steps);

        const long double elapsed = m_timer.elapsed();
        m_timer.update();

        const float delta = static_cast<float>((elapsed - m_timer.elapsed()) * 1e-9L);

        for (int i = 0; i < steps; ++i)
        {
            const float delta_stepped = i * delta * stepsinv;
            m_techniques[m_technique]->step(delta_stepped);
    }
    }

    void draw()
        {
        m_techniques[m_technique]->draw();
    }

    void reset()
    {
        m_timer.reset();

        for (auto technique : m_techniques)
            technique.second->reset();
    }

    // EVENT HANDLING

    virtual void keyPressEvent(KeyEvent & event)
    {
        switch (event.key())
        {
        case GLFW_KEY_C:
            debug() << "switch to Compute Shader Technique";
            m_technique = ComputeShaderTechnique;
            break;
        case GLFW_KEY_F:
            debug() << "switch to Fragment Shader Technique";
            m_technique = FragmentShaderTechnique;
            break;
        case GLFW_KEY_T:
            debug() << "switch to Transform Feedback Technique";
            m_technique = TransformFeedbackTechnique;
            break;
        case GLFW_KEY_P:       
            if (m_timer.paused())
            {
                debug() << "timer continue";
                m_timer.start();
            }
            else
            {
                debug() << "timer pause";
                m_timer.pause();
            }
            break;
        case GLFW_KEY_R:
            debug() << "timer reset";
            m_timer.reset();
            break;

        case GLFW_KEY_F5:
            FileRegistry::instance().reloadAll();
            break;
        }
    }

protected:
    
    enum ParticleTechnique
    {
        ComputeShaderTechnique
    ,   FragmentShaderTechnique
    ,   TransformFeedbackTechnique
    };

    ParticleTechnique m_technique;
    std::map<ParticleTechnique, AbstractParticleTechnique *> m_techniques;

    Timer m_timer;

    Camera * m_camera;
    int m_numParticles;

    Array<vec4> m_positions;
    Array<vec4> m_velocities;


    struct Attribute
    {
        int moep;
    };
    std::vector<Attribute> m_attributes;

    ref_ptr<Texture> m_forces;
};




/** This example shows ... .
*/
int main(int argc, char* argv[])
{
    ContextFormat format;

    Window window;
    window.setEventHandler(new EventHandler());

    window.create(format, "GPU - Particles Example");
    window.show();
    window.context()->setSwapInterval(Context::NoVerticalSyncronization);

    return MainLoop::run();
}
