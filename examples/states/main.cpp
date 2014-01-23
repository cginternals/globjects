
#include <GL/glew.h>

#include <glow/Error.h>
#include <glow/ref_ptr.h>
#include <glow/Buffer.h>
#include <glow/Array.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/debugmessageoutput.h>
#include <glow/State.h>
#include <glow/global.h>

#include <glowutils/File.h>
#include <glowutils/global.h>
#include <glowutils/StringTemplate.h>

#include <glowwindow/Context.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

#include <glow/logging.h>

using namespace glowwindow;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & ) override
    {
        glow::debugmessageoutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);
        CheckGLError();

        m_defaultLinesState = new glow::State();
        m_defaultLinesState->lineWidth(glow::getFloat(GL_LINE_WIDTH));
        m_thinnestLinesState = new glow::State();
        m_thinnestLinesState->lineWidth(0.2f);
        m_thinLinesState = new glow::State();
        m_thinLinesState->lineWidth(0.6f);
        m_normalLinesState = new glow::State();
        m_normalLinesState->lineWidth(1.0f);
        m_thickLinesState = new glow::State();
        m_thickLinesState->lineWidth(2.0f);

        m_vao = new glow::VertexArrayObject();
        m_buffer = new glow::Buffer(GL_ARRAY_BUFFER);

        glowutils::StringTemplate* vertexShaderSource = new glowutils::StringTemplate(new glowutils::File("data/states/standard.vert"));
        glowutils::StringTemplate* fragmentShaderSource = new glowutils::StringTemplate(new glowutils::File("data/states/standard.frag"));
        
#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        m_shaderProgram = new glow::Program();
        m_shaderProgram->attach(new glow::Shader(GL_VERTEX_SHADER, vertexShaderSource),
                                new glow::Shader(GL_FRAGMENT_SHADER, fragmentShaderSource));
        
        m_buffer->setData(glow::Array<glm::vec2>()
            << glm::vec2(-0.8, 0.8) << glm::vec2(0.8, 0.8)
            << glm::vec2(-0.8, 0.6) << glm::vec2(0.8, 0.6)
            << glm::vec2(-0.8, 0.4) << glm::vec2(0.8, 0.4)
            << glm::vec2(-0.8, 0.2) << glm::vec2(0.8, 0.2)
            << glm::vec2(-0.8, 0.0) << glm::vec2(0.8, 0.0)
            << glm::vec2(-0.8, -0.2) << glm::vec2(0.8, -0.2)
            << glm::vec2(-0.8, -0.4) << glm::vec2(0.8, -0.4)
            << glm::vec2(-0.8, -0.6) << glm::vec2(0.8, -0.6)
            << glm::vec2(-0.8, -0.8) << glm::vec2(0.8, -0.8)
            , GL_STATIC_DRAW
        );

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setBuffer(m_buffer, 0, sizeof(glm::vec2));
        m_vao->binding(0)->setFormat(2, GL_FLOAT);
        m_vao->enable(0);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        CheckGLError();
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CheckGLError();

        m_shaderProgram->use();

        m_defaultLinesState->apply();
        m_vao->drawArrays(GL_LINE, 0, 2);

        m_thinnestLinesState->apply();
        m_vao->drawArrays(GL_LINE, 2, 2);

        m_thinLinesState->apply();
        m_vao->drawArrays(GL_LINE, 4, 2);

        m_normalLinesState->apply();
        m_vao->drawArrays(GL_LINE, 6, 2);

        m_thickLinesState->apply();
        m_vao->drawArrays(GL_LINE, 8, 2);

        m_normalLinesState->apply();
        m_vao->drawArrays(GL_LINE, 10, 2);

        m_thinLinesState->apply();
        m_vao->drawArrays(GL_LINE, 12, 2);

        m_thinnestLinesState->apply();
        m_vao->drawArrays(GL_LINE, 14, 2);

        m_defaultLinesState->apply();
        m_vao->drawArrays(GL_LINE, 16, 2);

        m_shaderProgram->release();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_buffer;
    glow::ref_ptr<glow::Program> m_shaderProgram;

    glow::ref_ptr<glow::State> m_defaultLinesState;
    glow::ref_ptr<glow::State> m_thinnestLinesState;
    glow::ref_ptr<glow::State> m_thinLinesState;
    glow::ref_ptr<glow::State> m_normalLinesState;
    glow::ref_ptr<glow::State> m_thickLinesState;
};

int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 0);

    Window window;

    window.setEventHandler(new EventHandler());

    if (!window.create(format, "OpenGL States Example"))
    {
        return 1;
    }

    window.context()->setSwapInterval(Context::VerticalSyncronization);

    window.show();

    return MainLoop::run();
}
