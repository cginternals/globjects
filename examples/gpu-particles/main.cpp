
#include <GL/glew.h>

#include <algorithm>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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

#include <glowutils/File.h>
#include <glowutils/FileRegistry.h>
#include <glowutils/ScreenAlignedQuad.h>

#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glow;


class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
        : m_technique(ComputeShaderTechnique)
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

        // Initialize 3D Force Field (3D Texture)

        // Initialize Vertex, Geometry, and Fragment Shader for Particle Rendering


        // initialize Compute Based


        // initialize Fragment Based


        // initialize Transform Feedback Based

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
