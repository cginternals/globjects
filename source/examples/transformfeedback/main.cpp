
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/TransformFeedback.h>

#include <globjects/base/File.h>

#include <common/Timer.h>
#include <common/Window.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace glm;
using namespace globjects;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    : m_timer(false, true)
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        WindowEventHandler::initialize(window);

        if (!hasExtension(GLextension::GL_ARB_transform_feedback3))
        {
            critical() << "Transform feedback not supported.";

            window.close();
            return;
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        createAndSetupShaders();
        createAndSetupGeometry();
        createAndSetupTransformFeedback();

        m_timer.start();
    }

    void createAndSetupShaders()
    {
        m_vertexSource.reset(new File("data/transformfeedback/simple.vert"));
        m_vertexShader.reset(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()));

        m_fragmentSource.reset(new File("data/transformfeedback/simple.frag"));
        m_fragmentShader.reset(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()));

        m_program.reset(new Program());
        m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

        m_transformFeedbackSource.reset(new File("data/transformfeedback/transformfeedback.vert"));
        m_transformFeedbackShader.reset(new Shader(GL_VERTEX_SHADER, m_transformFeedbackSource.get()));

        m_transformFeedbackProgram.reset(new Program());
        m_transformFeedbackProgram->attach(m_transformFeedbackShader.get());

        m_modelViewUniform.reset(new Uniform<glm::mat4>("modelView", glm::mat4()));
        m_projectionUniform.reset(new Uniform<glm::mat4>("projection", glm::mat4()));
        m_deltaTUniform.reset(new Uniform<float>("deltaT", 0.0f));

        m_transformFeedbackProgram->attach(
            m_modelViewUniform.get(),
            m_projectionUniform.get(),
            m_deltaTUniform.get()
        );
    }

    void createAndSetupGeometry()
    {
        auto vertexArray = std::vector<vec4>({
            vec4(0, 0, 0, 1)
          , vec4(1, 0, 0, 1)
          , vec4(0, 1, 0, 1)
          , vec4(1, 0, 0, 1)
          , vec4(0, 1, 0, 1)
          , vec4(1, 1, 0, 1) });

        auto colorArray = std::vector<vec4>({
            vec4(1, 0, 0, 1)
          , vec4(1, 1, 0, 1)
          , vec4(0, 0, 1, 1)
          , vec4(1, 1, 0, 1)
          , vec4(0, 0, 1, 1)
          , vec4(0, 1, 0, 1) });

        m_vertexBuffer1.reset(new Buffer());
        m_vertexBuffer1->setData(vertexArray, GL_STATIC_DRAW);
        m_vertexBuffer2.reset(new Buffer());
        m_vertexBuffer2->setData(vertexArray, GL_STATIC_DRAW);
        m_colorBuffer.reset(new Buffer());
        m_colorBuffer->setData(colorArray, GL_STATIC_DRAW);

        m_vao.reset(new VertexArray());

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setFormat(4, GL_FLOAT);

        m_vao->binding(1)->setAttribute(1);
        m_vao->binding(1)->setBuffer(m_colorBuffer.get(), 0, sizeof(vec4));
        m_vao->binding(1)->setFormat(4, GL_FLOAT);

        m_vao->enable(0);
        m_vao->enable(1);
    }

    void createAndSetupTransformFeedback()
    {
        m_transformFeedback.reset(new TransformFeedback());
        m_transformFeedback->setVaryings(m_transformFeedbackProgram.get()
            , { { "next_position" } }, GL_INTERLEAVED_ATTRIBS);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        const int width = event.width();
        const int height = event.height();
        const int side = std::min<int>(width, height);

        glViewport((width - side) / 2, (height - side) / 2, side, side);

        m_projectionUniform->set(ortho(-0.4f, 1.4f, -0.4f, 1.4f, 0.f, 1.f));
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Buffer * drawBuffer  = m_vertexBuffer1.get();
        Buffer * writeBuffer = m_vertexBuffer2.get();

        m_vao->bind();

        m_deltaTUniform->set(static_cast<float>(m_timer.elapsed().count()) * float(std::nano::num) / float(std::nano::den));
        m_timer.reset();

        m_vao->binding(0)->setBuffer(drawBuffer, 0, sizeof(vec4));

        writeBuffer->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

        glEnable(GL_RASTERIZER_DISCARD);

        m_transformFeedbackProgram->use();
        m_transformFeedback->bind();
        m_transformFeedback->begin(GL_TRIANGLES);
        m_vao->drawArrays(GL_TRIANGLES, 0, 6);
        m_transformFeedback->end();
        m_transformFeedback->unbind();
        glDisable(GL_RASTERIZER_DISCARD);


        m_vao->binding(0)->setBuffer(writeBuffer, 0, sizeof(vec4));

        m_program->use();
        m_transformFeedback->draw(GL_TRIANGLE_STRIP);
        m_program->release();

        m_vao->unbind();

        std::swap(m_vertexBuffer1, m_vertexBuffer2);
    }

protected:
    std::unique_ptr<Program> m_program;
    std::unique_ptr<Program> m_transformFeedbackProgram;
    std::unique_ptr<AbstractStringSource> m_vertexSource;
    std::unique_ptr<AbstractStringSource> m_fragmentSource;
    std::unique_ptr<Shader> m_vertexShader;
    std::unique_ptr<Shader> m_fragmentShader;

    std::unique_ptr<AbstractStringSource> m_transformFeedbackSource;
    std::unique_ptr<Shader> m_transformFeedbackShader;
    
    std::unique_ptr<VertexArray> m_vao;

    std::unique_ptr<TransformFeedback> m_transformFeedback;
    
    std::unique_ptr<Buffer> m_vertexBuffer1;
    std::unique_ptr<Buffer> m_vertexBuffer2;
    std::unique_ptr<Buffer> m_colorBuffer;

    std::unique_ptr<Uniform<glm::mat4>> m_modelViewUniform;
    std::unique_ptr<Uniform<glm::mat4>> m_projectionUniform;
    std::unique_ptr<Uniform<float>> m_deltaTUniform;

    Timer m_timer;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"       << "Toggle vertical sync";
    info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 0);
    format.setProfile(ContextFormat::Profile::Core);
    format.setForwardCompatible(true);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Transform Feedback Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
