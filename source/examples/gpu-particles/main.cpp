#include <map>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/extension.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/Texture.h>
#include <globjects/NamedString.h>
#include <globjects/DebugMessage.h>

#include <globjects/base/File.h>

#include <common/Timer.h>
#include <common/Camera.h>
#include <common/AbstractCoordinateProvider.h>
#include <common/WorldInHandNavigation.h>
#include <common/Context.h>
#include <common/ContextFormat.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include "AbstractParticleTechnique.h"

#include "ComputeShaderParticles.h"
#include "FragmentShaderParticles.h"
#include "TransformFeedbackParticles.h"

#include <ExampleWindowEventHandler.h>


using namespace gl;
using namespace glm;

class EventHandler : public ExampleWindowEventHandler, AbstractCoordinateProvider
{
public:
    EventHandler(int numParticles)
    : m_technique(FragmentShaderTechnique)
    , m_numParticles(numParticles)
    , m_camera(nullptr)
    , m_steps(1)
    , m_nav(nullptr)
    {
        m_timer.setAutoUpdating(false);
        m_timer.start();
    }

    virtual ~EventHandler()
    {
        delete m_nav;
        delete m_camera;
    }

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        globjects::DebugMessage::enable();

        // Initialize Particle Positions and Attributes

        m_positions.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_positions[i] = vec4(sphericalRand<float>(1.f), 1.f);

        m_velocities.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_velocities[i] = vec4(0.f);


        m_forces = globjects::Texture::createDefault(GL_TEXTURE_3D);
        
        globjects::NamedString::create("/particle-step.inc", new globjects::File("data/gpu-particles/particle-step.inc"));

        // initialize camera

        m_camera = new Camera(vec3( 0.f, 1.f,-3.f));
        m_camera->setZNear(1.f);
        m_camera->setZFar(16.f);

        m_nav = new  WorldInHandNavigation();
        m_nav->setCamera(m_camera);
        m_nav->setCoordinateProvider(this);
        
        // initialize techniques

        if (globjects::hasExtension(GLextension::GL_ARB_compute_shader))
            m_techniques[ComputeShaderTechnique] = new ComputeShaderParticles(
                m_positions, m_velocities, *m_forces, *m_camera);
        else
            globjects::warning() << "Compute shader based implementation not supported.";

        if (globjects::hasExtension(GLextension::GL_ARB_transform_feedback3)) 
            m_techniques[TransformFeedbackTechnique] = new TransformFeedbackParticles(
                m_positions, m_velocities, *m_forces, *m_camera);
        else
            globjects::warning() << "Transform feedback based implementation not supported.";

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
        const long double elapsed = static_cast<long double>(m_timer.elapsed().count());
        m_timer.update();

        const float delta = static_cast<float>((m_timer.elapsed().count() - elapsed) * 1.0e-9L);

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
            const vec3 f(sphericalRand<float>(1.0));

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
            if (m_techniques[ComputeShaderTechnique]) 
            {
                globjects::debug() << "switch to compute shader technique";
                m_technique = ComputeShaderTechnique;
            };
            break;

        case GLFW_KEY_T:
            if (m_techniques[TransformFeedbackTechnique]) 
            {
                globjects::debug() << "switch to transform feedback technique";
                m_technique = TransformFeedbackTechnique;
            }
            break;

        case GLFW_KEY_F:
            globjects::debug() << "switch to fragment shader technique";
            m_technique = FragmentShaderTechnique;
            break;

        case GLFW_KEY_P:
            if (m_timer.paused())
            {
                globjects::debug() << "timer continue";
                m_timer.start();
            }
            else
            {
                globjects::debug() << "timer pause";
                m_timer.pause();
            }
            for (auto technique : m_techniques)
                if (technique.second)
                    technique.second->pause(m_timer.paused());

            break;

        case GLFW_KEY_R:
            reset(event.modifiers() & GLFW_MOD_SHIFT);
            break;

        case GLFW_KEY_MINUS:
            m_steps = max(1, m_steps - 1);
            globjects::debug() << "steps = " << m_steps;
            break;

        case GLFW_KEY_EQUAL: // bug? this is plus/add on my keyboard
            ++m_steps;
            globjects::debug() << "steps = " << m_steps;
            break;

        case GLFW_KEY_F5:
            globjects::File::reloadAll();
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
        case WorldInHandNavigation::RotateInteraction:
            m_nav->rotateProcess(event.pos());
            event.accept();
            break;

        case WorldInHandNavigation::PanInteraction:
        case WorldInHandNavigation::NoInteraction:
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
        if (WorldInHandNavigation::NoInteraction != m_nav->mode())
            return;

        m_nav->scaleAtCenter(-event.offset().y * 0.1f);
        event.accept();
    }

    virtual float depthAt(const ivec2 & /*windowCoordinates*/) const override
    {
        return 2.f;
    }

    virtual vec3 objAt(const ivec2 & /*windowCoordinates*/) const override
    {
        return vec3(0.f);
    }
    virtual vec3 objAt(const ivec2 & /*windowCoordinates*/, const float /*depth*/) const override
    {
        return vec3(0.f);
    }
    virtual vec3 objAt(const ivec2 & /*windowCoordinates*/, const float /*depth*/, const mat4 & /*viewProjectionInverted*/) const override
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

    Timer m_timer;

    int m_numParticles;
    Camera * m_camera;

    std::vector<vec4> m_positions;
    std::vector<vec4> m_velocities;

    int m_steps;

    WorldInHandNavigation * m_nav;

    struct Attribute
    {
        int moep;
    };
    std::vector<Attribute> m_attributes;

    globjects::ref_ptr<globjects::Texture> m_forces;
};


int main(int argc, char * argv[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    globjects::info() << "\t" << "Left Mouse" << "\t"  << "Rotate scene";
    globjects::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";
    globjects::info() << "\t" << "-" << "\t\t"         << "Reduce steps per frame";
    globjects::info() << "\t" << "=" << "\t\t"         << "Increase steps per frame";
    globjects::info() << "\t" << "R" << "\t\t"         << "Compute new forces";
    globjects::info() << "\t" << "Shift + R" << "\t"   << "Compute new forces and reset particles";
    globjects::info() << "\t" << "P" << "\t\t"         << "Toggle pause";
    globjects::info() << "\t" << "F" << "\t\t"         << "Particle computation using fragment shader";
    globjects::info() << "\t" << "T" << "\t\t"         << "Particle computation using transform feedback";
    globjects::info() << "\t" << "C" << "\t\t"         << "Particle computation using compute shader";

    ContextFormat format;
    format.setVersion(3, 3); // minimum required version is 3.3 due to particle drawing using geometry shader.
    format.setProfile(ContextFormat::CoreProfile);

    int numParticles = 262144;

    if (argc > 1) // assume that the argument is a number
        numParticles = atoi(argv[1]);

    Window window;
    window.setEventHandler(new EventHandler(numParticles));

    if (!window.create(format, "GPU - Particles Example"))
        return 1;

    window.context()->setSwapInterval(Context::NoVerticalSyncronization);
    window.show();

    return MainLoop::run();
}
