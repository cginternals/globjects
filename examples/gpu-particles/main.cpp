
#include <GL/glew.h>

#include <algorithm>
#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Texture.h>
//#include <glow/Uniform.h>
//#include <glow/Array.h>
//#include <glow/Program.h>
//#include <glow/Shader.h>
//#include <glow/Buffer.h>
//#include <glow/VertexArrayObject.h>
//#include <glow/VertexAttributeBinding.h>
#include <glowwindow/ContextFormat.h>
#include <glow/Error.h>
#include <glow/logging.h>

#include <glow/Timer.h>

#include <glowutils/MathMacros.h>
#include <glowutils/File.h>
#include <glowutils/FileRegistry.h>
#include <glowutils/ScreenAlignedQuad.h>

#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>


using namespace glow;
using namespace glm;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    : m_technique(ComputeShaderTechnique)
    , m_numParticles(100000)
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        DebugMessageOutput::enable();
        m_timer.reset();
        m_timer.start();

        // Initialize Commons

        // Initialize Particle Positions and Attributes

        m_positions.resize(m_numParticles);
        for (int i = 0; i < m_numParticles; ++i)
            m_positions[i] = vec3(randf(-1.f, +1.f), randf(-1.f, +1.f), randf(-1.f, +1.f));


        m_attributes.resize(m_numParticles);
        Attribute attribute;
        for (int i = 0; i < m_numParticles; ++i)
        {
            // ToDo:
            attribute.moep = 0;

            m_attributes[i] = attribute;
        }


        // Initialize 3D Force Field (3D Texture)

        static const ivec3 fdim(16, 16, 16); //  // 4k * 3

        std::vector<vec3> forces;
        forces.resize(fdim.x * fdim.y * fdim.z);

        for (int z = 0; z < fdim.z; ++z)
        for (int y = 0; y < fdim.y; ++y)
        for (int x = 0; x < fdim.x; ++x)
        {
            const int i = z *  fdim.x * fdim.y + y * fdim.x + x;
            const vec3 f(randf(-1.0, +1.f), randf(-1.0, +1.f), randf(-1.0, +1.f));

            forces[i] = f;
        }
        
        m_forcesTex = new Texture(GL_TEXTURE_3D);

        m_forcesTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_forcesTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        m_forcesTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        m_forcesTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_forcesTex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        m_forcesTex->image3D(0, GL_RGB32F, fdim.x, fdim.y, fdim.z, 0, GL_RGB, GL_FLOAT, forces.data());

        
        // Initialize Vertex, Geometry, and Fragment Shader for Particle Rendering


        // initialize Compute Based

            // TODO: init particle pos storage

        // initialize Fragment Based

            // TODO: init particle pos storage

        // initialize Transform Feedback Based

            // TODO: init particle pos storage
    }
    
    virtual void resizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        particleStep();

	    //m_computeProgram->setUniform("roll", static_cast<float>(m_frame) * 0.01f);
	    //m_texture->bind();

	    //m_computeProgram->use();
	    //glDispatchCompute(512/16, 512/16, 1); // 512^2 threads in blocks of 16^2
	    //m_computeProgram->release();

        //m_quad->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    void particleStep()
    {
        static const int steps = 1;
        static const float stepsinv = 1.f / static_cast<float>(steps);

        const long double elapsed = m_timer.elapsed();
        m_timer.update();

        const float delta = static_cast<float>((elapsed - m_timer.elapsed()) * 1e-9L);

        for (int i = 0; i < steps; ++i)
        {
            const float delta_stepped = i * delta * stepsinv;

            switch (m_technique)
            {
            case ComputeShaderTechnique:
                particleStepCompute(delta_stepped); break;
            case FragmentShaderTechnique:
                particleStepFragment(delta_stepped); break;
            case TransformFeedbackTechnique:
                particleStepTransformFeedback(delta_stepped); break;
            default:
                break;
            }
        }
    }

    void particleStepCompute(const float delta)
    {

    }

    void particleStepFragment(const float delta)
    {

    }

    void particleStepTransformFeedback(const float delta)
    {

    }

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
        }
    }

protected:
    
    enum ParticleTechnique
    {
        ComputeShaderTechnique
        , FragmentShaderTechnique
        , TransformFeedbackTechnique
    };

    ParticleTechnique m_technique;
    Timer m_timer;

    int m_numParticles;

    std::vector<vec3> m_positions;

    struct Attribute
    {
        int moep;
    };

    std::vector<Attribute> m_attributes;

    ref_ptr<Texture> m_forcesTex;
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
