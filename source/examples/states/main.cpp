
#include <glowbase/ref_ptr.h>

#include <glow/Buffer.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/debugmessageoutput.h>
#include <glow/State.h>
#include <glow/glow.h>

#include <glowbase/File.h>
#include <glowutils/glowutils.h>
#include <glowutils/StringTemplate.h>

#include <glowwindow/Context.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <glow/logging.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;

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

        glow::debugmessageoutput::enable();

        gl::ClearColor(0.2f, 0.3f, 0.4f, 1.f);


        m_defaultPointSizeState = new glow::State();
        m_defaultPointSizeState->pointSize(glow::getFloat(gl::POINT_SIZE));
        m_thinnestPointSizeState = new glow::State();
        m_thinnestPointSizeState->pointSize(2.0f);
        m_thinPointSizeState = new glow::State();
        m_thinPointSizeState->pointSize(5.0f);
        m_normalPointSizeState = new glow::State();
        m_normalPointSizeState->pointSize(10.0f);
        m_thickPointSizeState = new glow::State();
        m_thickPointSizeState->pointSize(20.0f);
        m_disableRasterizerState = new glow::State();
        m_disableRasterizerState->enable(gl::RASTERIZER_DISCARD);
        m_enableRasterizerState = new glow::State();
        m_enableRasterizerState->disable(gl::RASTERIZER_DISCARD);

        m_vao = new glow::VertexArrayObject();
        m_buffer = new glow::Buffer();

        glowutils::StringTemplate* vertexShaderSource = new glowutils::StringTemplate(new glow::File("data/states/standard.vert"));
        glowutils::StringTemplate* fragmentShaderSource = new glowutils::StringTemplate(new glow::File("data/states/standard.frag"));
        
#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        m_shaderProgram = new glow::Program();
        m_shaderProgram->attach(new glow::Shader(gl::VERTEX_SHADER, vertexShaderSource),
                                new glow::Shader(gl::FRAGMENT_SHADER, fragmentShaderSource));
        
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
            , gl::STATIC_DRAW
        );

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setBuffer(m_buffer, 0, sizeof(glm::vec2));
        m_vao->binding(0)->setFormat(2, gl::FLOAT);
        m_vao->enable(0);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        gl::Viewport(0, 0, event.width(), event.height());

    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


        m_shaderProgram->use();

        m_defaultPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 0, 5);

        m_thinnestPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 5, 5);

        m_thinPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 10, 5);

        m_normalPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 15, 5);

        m_thickPointSizeState->apply();

        m_vao->drawArrays(gl::POINTS, 20, 1);
        m_disableRasterizerState->apply();
        m_vao->drawArrays(gl::POINTS, 21, 1);
        m_enableRasterizerState->apply();
        m_vao->drawArrays(gl::POINTS, 22, 1);
        m_disableRasterizerState->apply();
        m_vao->drawArrays(gl::POINTS, 23, 1);
        m_enableRasterizerState->apply();
        m_vao->drawArrays(gl::POINTS, 24, 1);

        m_normalPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 25, 5);

        m_thinPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 30, 5);

        m_thinnestPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 35, 5);

        m_defaultPointSizeState->apply();
        m_vao->drawArrays(gl::POINTS, 35, 5);

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

    glow::ref_ptr<glow::State> m_defaultPointSizeState;
    glow::ref_ptr<glow::State> m_thinnestPointSizeState;
    glow::ref_ptr<glow::State> m_thinPointSizeState;
    glow::ref_ptr<glow::State> m_normalPointSizeState;
    glow::ref_ptr<glow::State> m_thickPointSizeState;
    glow::ref_ptr<glow::State> m_disableRasterizerState;
    glow::ref_ptr<glow::State> m_enableRasterizerState;
};

int main(int /*argc*/, char* /*argv*/[])
{
    glow::info() << "Usage:";
    glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";

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
