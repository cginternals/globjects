
#include <GL/glew.h>

#include <algorithm>
#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Texture.h>
#include <glow/Uniform.h>
#include <glow/Array.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/TransformFeedback.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glowwindow/ContextFormat.h>
#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/Timer.h>

#include <glowutils\FileRegistry.h>
#include <glowutils/Camera.h>
#include <glowutils/MathMacros.h>
#include <glowutils/File.h>
#include <glowutils/FileRegistry.h>
#include <glowutils/ScreenAlignedQuad.h>

#include <glowwindow/Context.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>


using namespace glow;
using namespace glm;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    : m_technique(ComputeShaderTechnique)
    , m_numParticles(10000)
    , m_camera(nullptr)
    {
    }

    virtual ~EventHandler()
    {
        delete m_camera;
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
            m_positions[i] = vec4(randf(-1.f, +1.f), randf(-1.f, +1.f), randf(-1.f, +1.f), 0.f);


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

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        m_camera = new Camera(vec3(0.f, 0.f, -4.f));

        m_renderProgram = new Program();
        m_renderProgram->attach(
            createShaderFromFile(GL_VERTEX_SHADER, "data/gpu-particles/points.vert")
        ,   createShaderFromFile(GL_GEOMETRY_SHADER, "data/gpu-particles/points.geom")
        ,   createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/points.frag"));
        

        // initialize Compute Based

            // TODO: init particle pos storage
        m_computeProgram = new Program();
        m_computeProgram->attach(
            createShaderFromFile(GL_COMPUTE_SHADER, "data/gpu-particles/particle.comp"));        

        // initialize Fragment Based

            // TODO: init particle pos storage

        // initialize Transform Feedback Based

        m_transformFeedbackProgram = new glow::Program();
        m_transformFeedbackProgram->attach(glow::createShaderFromFile(GL_VERTEX_SHADER, "data/gpu-particles/transformfeedback.vert"));
        m_transformFeedbackProgram->setUniform("deltaT", 0.0f);
        m_transformFeedbackVertexBuffer1 = new glow::Buffer(GL_ARRAY_BUFFER);
        m_transformFeedbackVertexBuffer1->setData(glow::Array<glm::vec4>(m_positions));
        m_transformFeedbackVertexBuffer2 = new glow::Buffer(GL_ARRAY_BUFFER);
        m_transformFeedbackVertexBuffer2->setData(glow::Array<glm::vec4>(m_positions));

        m_transformFeedback = new glow::TransformFeedback();
        m_transformFeedback->setVaryings(m_transformFeedbackProgram, glow::Array<const char*>{ "next_position" }, GL_INTERLEAVED_ATTRIBS);

        m_transformFeedbackVAO = new glow::VertexArrayObject();
        m_transformFeedbackVAO->binding(0)->setAttribute(m_transformFeedbackProgram->getAttributeLocation("in_position"));
        m_transformFeedbackVAO->binding(0)->setFormat(3, GL_FLOAT);
        m_transformFeedbackVAO->enable(m_transformFeedbackProgram->getAttributeLocation("in_position"));
            // TODO: init particle pos storage


        // TEST DATA

        m_vao = new VertexArrayObject();
        m_vao->bind();

        m_vertices = new Buffer(GL_ARRAY_BUFFER);
        m_vertices->setData(m_positions.size() * sizeof(vec4), m_positions.data());

        auto vertexBinding = m_vao->binding(0);
        vertexBinding->setAttribute(0);
        vertexBinding->setBuffer(m_vertices, 0, sizeof(vec4));
        vertexBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);
        m_vao->enable(0);
    }
    
    virtual void resizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        m_camera->setViewport(event.size());
        m_renderProgram->setUniform("aspect", m_camera->aspectRatio());
    }

    virtual void idle(Window & window) override
    {
        float f = static_cast<float>(m_timer.elapsed() * 1e-10);
        m_camera->setEye(vec3(cos(f), 0.f, sin(f)) * 4.f);

        particleStep();
        window.repaint();
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        particleDraw();
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
        m_computeProgram->use();
        m_vertices->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
        m_computeProgram->dispatchCompute(512 / 16, 1, 1);
        m_computeProgram->release();

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    void particleStepFragment(const float delta)
    {

    }

    void particleStepTransformFeedback(const float delta)
    {
        m_transformFeedbackVAO->bind();

        m_transformFeedbackProgram->setUniform("deltaT", delta);

        m_transformFeedbackVAO->disable(m_transformFeedbackProgram->getAttributeLocation("in_position"));
        m_transformFeedbackVAO->binding(0)->setBuffer(m_transformFeedbackVertexBuffer1, 0, sizeof(glm::vec3));
        m_transformFeedbackVAO->enable(m_transformFeedbackProgram->getAttributeLocation("in_position"));

        m_transformFeedbackVertexBuffer2->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

        glEnable(GL_RASTERIZER_DISCARD);
        m_transformFeedbackProgram->use();
        m_transformFeedback->bind();
        m_transformFeedback->begin(GL_POINTS);
        m_transformFeedbackVAO->drawArrays(GL_POINTS, 0, m_numParticles);
        m_transformFeedback->end();
        m_transformFeedback->unbind();
        glDisable(GL_RASTERIZER_DISCARD);

        m_transformFeedbackVAO->unbind();

        std::swap(m_transformFeedbackVertexBuffer1, m_transformFeedbackVertexBuffer2);
    }

    void particleDraw()
    {
        switch (m_technique)
        {
        case ComputeShaderTechnique:
            particleDrawCompute(); break;
        case FragmentShaderTechnique:
            particleDrawFragment(); break;
        case TransformFeedbackTechnique:
            particleDrawTransformFeedback(); break;
        default:
            break;
        }
    }

    void particleDrawCompute()
    {
        glEnable(GL_BLEND);

        m_renderProgram->use();
        m_renderProgram->setUniform("viewProjection", m_camera->viewProjection());

        m_vao->drawArrays(GL_POINTS, 0, m_numParticles);

        m_renderProgram->release();

        glDisable(GL_BLEND);
    }

    void particleDrawFragment()
    {
        particleDrawCompute();
    }

    void particleDrawTransformFeedback()
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

        case GLFW_KEY_F5:
            FileRegistry::instance().reloadAll();
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

    std::vector<vec4> m_positions;

    struct Attribute
    {
        int moep;
    };

    std::vector<Attribute> m_attributes;

    ref_ptr<Texture> m_forcesTex;

    ref_ptr<VertexArrayObject> m_vao;
    ref_ptr<Buffer> m_vertices;
    ref_ptr<Program> m_renderProgram;
    ref_ptr<Program> m_computeProgram;

    Camera * m_camera;

    // Transform Feedback Members
    glow::ref_ptr<glow::Program> m_transformFeedbackProgram;
    glow::ref_ptr<glow::TransformFeedback> m_transformFeedback;
    glow::ref_ptr<glow::Buffer> m_transformFeedbackVertexBuffer1;
    glow::ref_ptr<glow::Buffer> m_transformFeedbackVertexBuffer2;
    glow::ref_ptr<glow::VertexArrayObject> m_transformFeedbackVAO;
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
