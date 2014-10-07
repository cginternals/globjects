
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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

        if (!globjects::hasExtension(GLextension::GL_ARB_transform_feedback3))
        {
            globjects::critical() << "Transform feedback not supported.";

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
        m_shaderProgram = new globjects::Program();
        m_shaderProgram->attach(
            globjects::Shader::fromFile(GL_VERTEX_SHADER,   "data/transformfeedback/simple.vert")
          , globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/transformfeedback/simple.frag"));

        m_transformFeedbackProgram = new globjects::Program();
        m_transformFeedbackProgram->attach(
            globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/transformfeedback/transformfeedback.vert"));

        m_transformFeedbackProgram->setUniform("deltaT", 0.0f);
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

        m_vertexBuffer1 = new globjects::Buffer();
        m_vertexBuffer1->setData(vertexArray, GL_STATIC_DRAW);
        m_vertexBuffer2 = new globjects::Buffer();
        m_vertexBuffer2->setData(vertexArray, GL_STATIC_DRAW);
        m_colorBuffer = new globjects::Buffer();
        m_colorBuffer->setData(colorArray, GL_STATIC_DRAW);

        m_vao = new globjects::VertexArray();

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setFormat(4, GL_FLOAT);

        m_vao->binding(1)->setAttribute(1);
        m_vao->binding(1)->setBuffer(m_colorBuffer, 0, sizeof(vec4));
        m_vao->binding(1)->setFormat(4, GL_FLOAT);

        m_vao->enable(0);
        m_vao->enable(1);
    }

    void createAndSetupTransformFeedback()
    {
        m_transformFeedback = new globjects::TransformFeedback();
        m_transformFeedback->setVaryings(m_transformFeedbackProgram
            , std::array<const char*, 1>{ { "next_position" } }, GL_INTERLEAVED_ATTRIBS);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        const int width = event.width();
        const int height = event.height();
        const int side = std::min<int>(width, height);

        glViewport((width - side) / 2, (height - side) / 2, side, side);

	    m_shaderProgram->setUniform("modelView", mat4());
        m_shaderProgram->setUniform("projection", ortho(-0.4f, 1.4f, -0.4f, 1.4f, 0.f, 1.f));
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        globjects::Buffer * drawBuffer  = m_vertexBuffer1;
        globjects::Buffer * writeBuffer = m_vertexBuffer2;

        m_vao->bind();

        m_transformFeedbackProgram->setUniform("deltaT", static_cast<float>(m_timer.elapsed().count()) * float(std::nano::num) / float(std::nano::den));
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

        m_shaderProgram->use();
        m_transformFeedback->draw(GL_TRIANGLE_STRIP);
        m_shaderProgram->release();

        m_vao->unbind();

        std::swap(m_vertexBuffer1, m_vertexBuffer2);
    }

protected:
    globjects::ref_ptr<globjects::Program> m_shaderProgram;
    globjects::ref_ptr<globjects::Program> m_transformFeedbackProgram;
	
    globjects::ref_ptr<globjects::VertexArray> m_vao;

    globjects::ref_ptr<globjects::TransformFeedback> m_transformFeedback;
	
    globjects::ref_ptr<globjects::Buffer> m_vertexBuffer1;
    globjects::ref_ptr<globjects::Buffer> m_vertexBuffer2;
    globjects::ref_ptr<globjects::Buffer> m_colorBuffer;

    Timer m_timer;
};


int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    globjects::info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 0);
    format.setProfile(ContextFormat::CoreProfile);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Transform Feedback Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
