#include <map>

#include <GL/glew.h>

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/Texture.h>
#include <glow/global.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/Camera.h>
#include <glowutils/File.h>
#include <glowutils/File.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/Timer.h>

#include <glowwindow/Context.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include "AbstractParticleTechnique.h"

#include "ComputeShaderParticles.h"
#include "FragmentShaderParticles.h"
#include "TransformFeedbackParticles.h"

#include <ExampleWindowEventHandler.h>


using namespace glowwindow;
using namespace glm;


class EventHandler : public ExampleWindowEventHandler, glowutils::AbstractCoordinateProvider
{
public:
    EventHandler()
    : m_technique(FragmentShaderTechnique)
    , m_numParticles(1000000)
    , m_camera(nullptr)
    , m_steps(1)
    , m_nav(nullptr)
    {
        m_timer.setAutoUpdating(false);
        m_timer.start();

        // Initialize Particle Positions and Attributes

        m_positions.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_positions[i] = vec4(glm::sphericalRand<float>(1.0), 1.f);

        m_velocities.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_velocities[i] = vec4(0.f);

        //m_attributes.resize(m_numParticles);
        //Attribute attribute;
        //for (int i = 0; i < m_numParticles; ++i)
        //{
        //    // ToDo:
        //    attribute.moep = 0;

        //    m_attributes[i] = attribute;
        //}
    }

    virtual ~EventHandler()
    {
        delete m_nav;
        delete m_camera;
    }

    virtual void initialize(Window & ) override
    {
        glow::debugmessageoutput::enable();

        m_forces = new glow::Texture(GL_TEXTURE_3D);

        m_forces->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_forces->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        m_forces->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        m_forces->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_forces->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Initialize shader includes

        glow::createNamedString("/glow/data/gpu-particles/particleMovement.inc", new glowutils::File("data/gpu-particles/particleMovement.inc"));
        
        // initialize camera

        m_camera = new glowutils::Camera(vec3(0.f, 1.f, -3.f));
        m_camera->setZNear(0.1f);
        m_camera->setZFar(16.f);

        m_nav = new  glowutils::WorldInHandNavigation();
        m_nav->setCamera(m_camera);
        m_nav->setCoordinateProvider(this);
        
        // initialize techniques

        // TODO: Implement a better way to check if a feature is supported
        if (GLEW_ARB_compute_shader) {
            m_techniques[ComputeShaderTechnique] = new ComputeShaderParticles(
                m_positions, m_velocities, *m_forces, *m_camera);
        }
        if (GLEW_ARB_transform_feedback3) {
            m_techniques[TransformFeedbackTechnique] = new TransformFeedbackParticles(
                m_positions, m_velocities, *m_forces, *m_camera);
        }

        m_techniques[FragmentShaderTechnique] = new FragmentShaderParticles(
            m_positions, m_velocities, *m_forces, *m_camera);

        for (auto technique : m_techniques)
            if (technique.second)
                technique.second->initialize();

        reset();
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        CheckGLError();

        m_camera->setViewport(event.size());

        for (auto technique : m_techniques)
            technique.second->resize();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void paintEvent(PaintEvent &) override
    {
        draw();
    }

    void step(const float delta)
    {
        const float delta_stepped = delta / static_cast<float>(m_steps);

        for (int i = 0; i < m_steps; ++i)
            m_techniques[m_technique]->step(delta_stepped);
    }

    void draw()
    {
        const long double elapsed = m_timer.elapsed();
        m_timer.update();

        const float delta = static_cast<float>((m_timer.elapsed() - elapsed) * 1.0e-9L);

        step(delta); // requires context to be current
        m_techniques[m_technique]->draw(delta);
    }

    void reset(const bool particles = true)
    {
        // initialize 3D Force Field (3D Texture)

        static const ivec3 fdim(5, 5, 5); // this has center axises and allows for random rings etc..

        std::vector<vec3> forces;
        forces.resize(fdim.x * fdim.y * fdim.z);

        srand(static_cast<unsigned int>(time(0)));

        for (int z = 0; z < fdim.z; ++z)
        for (int y = 0; y < fdim.y; ++y)
        for (int x = 0; x < fdim.x; ++x)
        {
            const int i = z *  fdim.x * fdim.y + y * fdim.x + x;
            const vec3 f(glm::sphericalRand<float>(1.0));

            forces[i] = f * (1.f - length(vec3(x, y, z)) / std::sqrt(3.f));
        }

        m_forces->image3D(0, GL_RGB32F, fdim.x, fdim.y, fdim.z, 0, GL_RGB, GL_FLOAT, forces.data());

        if (!particles)
            return;

        m_timer.reset();
        m_timer.update();

        for (auto technique : m_techniques)
            technique.second->reset();
    }

    // EVENT HANDLING

    virtual void keyPressEvent(KeyEvent & event) override
    {
        switch (event.key())
        {
        case GLFW_KEY_C:
            if (m_techniques[ComputeShaderTechnique]) {
                glow::debug() << "switch to compute shader technique";
                m_technique = ComputeShaderTechnique;
            } else glow::debug() << "compute shader technique not available";
            break;
        case GLFW_KEY_T:
            if (m_techniques[TransformFeedbackTechnique]) {
                glow::debug() << "switch to transform feedback technique";
                m_technique = TransformFeedbackTechnique;
            } else glow::debug() << "transform feedback technique not available";
            break;
        case GLFW_KEY_F:
            glow::debug() << "switch to fragment shader technique";
            m_technique = FragmentShaderTechnique;
            break;

        case GLFW_KEY_P:       
            if (m_timer.paused())
            {
                glow::debug() << "timer continue";
                m_timer.start();
            }
            else
            {
                glow::debug() << "timer pause";
                m_timer.pause();
            }
            break;

        case GLFW_KEY_R:
            reset(event.modifiers() & GLFW_MOD_SHIFT);
            break;

        case GLFW_KEY_MINUS:
            m_steps = max(1, m_steps - 1);
            glow::debug() << "steps = " << m_steps;
            break;

        case GLFW_KEY_EQUAL: // bug? this is plus/add on my keyboard
            ++m_steps;
            glow::debug() << "steps = " << m_steps;
            break;

        case GLFW_KEY_F5:
            glowutils::File::reloadAll();
            break;
        }
    }

    virtual void mousePressEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_nav->rotateBegin(event.pos());
            event.accept();
            break;
        }
    }
    virtual void mouseMoveEvent(MouseEvent & event) override
    {
        switch (m_nav->mode())
        {
        case glowutils::WorldInHandNavigation::RotateInteraction:
            m_nav->rotateProcess(event.pos());
            event.accept();
            break;
        case glowutils::WorldInHandNavigation::PanInteraction:
        case glowutils::WorldInHandNavigation::NoInteraction:
            break;
        }
    }
    virtual void mouseReleaseEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_nav->rotateEnd();
            event.accept();
            break;
        }
    }

    virtual void scrollEvent(ScrollEvent & event) override
    {
        if (glowutils::WorldInHandNavigation::NoInteraction != m_nav->mode())
            return;

        m_nav->scaleAtCenter(-event.offset().y * 0.1f);
        event.accept();
    }

    virtual float depthAt(const ivec2 & /*windowCoordinates*/) const override
    {
        return 2.0;
    }

    virtual vec3 objAt(const ivec2 & /*windowCoordinates*/) const override
    {
        return vec3(0.f);
    }
    virtual vec3 objAt(const ivec2 & /*windowCoordinates*/, const float /*depth*/) const override
    {
        return vec3(0.f);
    }
    virtual glm::vec3 objAt(const ivec2 & /*windowCoordinates*/, const float /*depth*/, const mat4 & /*viewProjectionInverted*/) const override
    {
        return vec3(0.f);
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

    glowutils::Timer m_timer;

    int m_numParticles;
    glowutils::Camera * m_camera;

    std::vector<vec4> m_positions;
    std::vector<vec4> m_velocities;

    int m_steps;

    glowutils::WorldInHandNavigation * m_nav;

    struct Attribute
    {
        int moep;
    };
    std::vector<Attribute> m_attributes;

    glow::ref_ptr<glow::Texture> m_forces;
};




/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glow::info() << "Usage:";
    glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glow::info() << "\t" << "Left Mouse" << "\t" << "Rotate scene";
    glow::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";
    glow::info() << "\t" << "-" << "\t\t" << "Reduce steps per frame";
    glow::info() << "\t" << "=" << "\t\t" << "Increase steps per frame";
    glow::info() << "\t" << "R" << "\t\t" << "Compute new forces";
    glow::info() << "\t" << "Shift + R" << "\t" << "Compute new forces and reset particles";
    glow::info() << "\t" << "P" << "\t\t" << "Toggle pause";
    glow::info() << "\t" << "F" << "\t\t" << "Particle computation using fragment shader";
    glow::info() << "\t" << "T" << "\t\t" << "Particle computation using transform feedback";
    glow::info() << "\t" << "C" << "\t\t" << "Particle computation using compute shader";

    ContextFormat format;
    format.setVersion(3, 3); // minimum required version is 3.3 due to particle drawing using geometry shader.
    format.setProfile(ContextFormat::CoreProfile);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "GPU - Particles Example"))
    {
        window.context()->setSwapInterval(Context::NoVerticalSyncronization);

        window.show();

        return MainLoop::run();
    }
    else
    {
        return 1;
    }
}
