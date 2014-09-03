
#include <glbinding/gl/gl.h>

#include <globjects-base/ref_ptr.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/DebugMessage.h>
#include <globjects/State.h>
#include <globjects/globjects.h>

#include <globjects-base/File.h>
#include <globjects-utils/globjects-utils.h>
#include <globjects-utils/StringTemplate.h>

#include <common/Context.h>
#include <common/ContextFormat.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <globjects/logging.h>

#include <ExampleWindowEventHandler.h>



class EventHandler : public ExampleWindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glo::DebugMessage::enable();

        gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);


        m_defaultPointSizeState = new glo::State();
        m_defaultPointSizeState->pointSize(glo::getFloat(gl::GL_POINT_SIZE));
        m_thinnestPointSizeState = new glo::State();
        m_thinnestPointSizeState->pointSize(2.0f);
        m_thinPointSizeState = new glo::State();
        m_thinPointSizeState->pointSize(5.0f);
        m_normalPointSizeState = new glo::State();
        m_normalPointSizeState->pointSize(10.0f);
        m_thickPointSizeState = new glo::State();
        m_thickPointSizeState->pointSize(20.0f);
        m_disableRasterizerState = new glo::State();
        m_disableRasterizerState->enable(gl::GL_RASTERIZER_DISCARD);
        m_enableRasterizerState = new glo::State();
        m_enableRasterizerState->disable(gl::GL_RASTERIZER_DISCARD);

        m_vao = new glo::VertexArray();
        m_buffer = new glo::Buffer();

        gloutils::StringTemplate* vertexShaderSource = new gloutils::StringTemplate(new glo::File("data/states/standard.vert"));
        gloutils::StringTemplate* fragmentShaderSource = new gloutils::StringTemplate(new glo::File("data/states/standard.frag"));
        
#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        m_shaderProgram = new glo::Program();
        m_shaderProgram->attach(new glo::Shader(gl::GL_VERTEX_SHADER, vertexShaderSource),
                                new glo::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource));
        
        m_buffer->setData(std::vector<glm::vec2>({
              glm::vec2(-0.8, 0.8), glm::vec2(-0.4, 0.8), glm::vec2(0.0, 0.8), glm::vec2(0.4, 0.8), glm::vec2(0.8, 0.8)
            , glm::vec2(-0.8, 0.6), glm::vec2(-0.4, 0.6), glm::vec2(0.0, 0.6), glm::vec2(0.4, 0.6), glm::vec2(0.8, 0.6)
            , glm::vec2(-0.8, 0.4), glm::vec2(-0.4, 0.4), glm::vec2(0.0, 0.4), glm::vec2(0.4, 0.4), glm::vec2(0.8, 0.4)
            , glm::vec2(-0.8, 0.2), glm::vec2(-0.4, 0.2), glm::vec2(0.0, 0.2), glm::vec2(0.4, 0.2), glm::vec2(0.8, 0.2)
            , glm::vec2(-0.8, 0.0), glm::vec2(-0.4, 0.0), glm::vec2(0.0, 0.0), glm::vec2(0.4, 0.0), glm::vec2(0.8, 0.0)
            , glm::vec2(-0.8, -0.2), glm::vec2(-0.4, -0.2), glm::vec2(0.0, -0.2), glm::vec2(0.4, -0.2), glm::vec2(0.8, -0.2)
            , glm::vec2(-0.8, -0.4), glm::vec2(-0.4, -0.4), glm::vec2(0.0, -0.4), glm::vec2(0.4, -0.4), glm::vec2(0.8, -0.4)
            , glm::vec2(-0.8, -0.6), glm::vec2(-0.4, -0.6), glm::vec2(0.0, -0.6), glm::vec2(0.4, -0.6), glm::vec2(0.8, -0.6)
            , glm::vec2(-0.8, -0.8), glm::vec2(-0.4, -0.8), glm::vec2(0.0, -0.8), glm::vec2(0.4, -0.8), glm::vec2(0.8, -0.8)
            })
            , gl::GL_STATIC_DRAW
        );

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setBuffer(m_buffer, 0, sizeof(glm::vec2));
        m_vao->binding(0)->setFormat(2, gl::GL_FLOAT);
        m_vao->enable(0);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        gl::glViewport(0, 0, event.width(), event.height());

    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);


        m_shaderProgram->use();

        m_defaultPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 0, 5);

        m_thinnestPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 5, 5);

        m_thinPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 10, 5);

        m_normalPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 15, 5);

        m_thickPointSizeState->apply();

        m_vao->drawArrays(gl::GL_POINTS, 20, 1);
        m_disableRasterizerState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 21, 1);
        m_enableRasterizerState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 22, 1);
        m_disableRasterizerState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 23, 1);
        m_enableRasterizerState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 24, 1);

        m_normalPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 25, 5);

        m_thinPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 30, 5);

        m_thinnestPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 35, 5);

        m_defaultPointSizeState->apply();
        m_vao->drawArrays(gl::GL_POINTS, 35, 5);

        m_shaderProgram->release();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }
protected:
    glo::ref_ptr<glo::VertexArray> m_vao;
    glo::ref_ptr<glo::Buffer> m_buffer;
    glo::ref_ptr<glo::Program> m_shaderProgram;

    glo::ref_ptr<glo::State> m_defaultPointSizeState;
    glo::ref_ptr<glo::State> m_thinnestPointSizeState;
    glo::ref_ptr<glo::State> m_thinPointSizeState;
    glo::ref_ptr<glo::State> m_normalPointSizeState;
    glo::ref_ptr<glo::State> m_thickPointSizeState;
    glo::ref_ptr<glo::State> m_disableRasterizerState;
    glo::ref_ptr<glo::State> m_enableRasterizerState;
};

int main(int /*argc*/, char* /*argv*/[])
{
    glo::info() << "Usage:";
    glo::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glo::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";

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
