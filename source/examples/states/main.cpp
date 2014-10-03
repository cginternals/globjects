
#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/DebugMessage.h>
#include <globjects/State.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/File.h>

#include <common/StringTemplate.h>
#include <common/Context.h>
#include <common/ContextFormat.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>


using namespace gl;
using namespace glm;

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

        globjects::DebugMessage::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        m_defaultPointSizeState  = new globjects::State();
        m_defaultPointSizeState->pointSize(globjects::getFloat(GL_POINT_SIZE));
        m_thinnestPointSizeState = new globjects::State();
        m_thinnestPointSizeState->pointSize(2.0f);
        m_thinPointSizeState     = new globjects::State();
        m_thinPointSizeState->pointSize(5.0f);
        m_normalPointSizeState   = new globjects::State();
        m_normalPointSizeState->pointSize(10.0f);
        m_thickPointSizeState    = new globjects::State();
        m_thickPointSizeState->pointSize(20.0f);
        m_disableRasterizerState = new globjects::State();
        m_disableRasterizerState->enable(GL_RASTERIZER_DISCARD);
        m_enableRasterizerState  = new globjects::State();
        m_enableRasterizerState->disable(GL_RASTERIZER_DISCARD);

        m_vao = new globjects::VertexArray();
        m_buffer = new globjects::Buffer();

        StringTemplate * vertexShaderSource  = new StringTemplate (new globjects::File("data/states/standard.vert"));
        StringTemplate * fragmentShaderSource = new StringTemplate(new globjects::File("data/states/standard.frag"));
        
#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        m_shaderProgram = new globjects::Program();
        m_shaderProgram->attach(new globjects::Shader(GL_VERTEX_SHADER, vertexShaderSource),
                                new globjects::Shader(GL_FRAGMENT_SHADER, fragmentShaderSource));
        
        m_buffer->setData(std::vector<vec2>({
            vec2(-0.8f, 0.8f), vec2(-0.4f, 0.8f), vec2( 0.0f, 0.8f), vec2( 0.4f, 0.8f), vec2( 0.8f, 0.8f)
          , vec2(-0.8f, 0.6f), vec2(-0.4f, 0.6f), vec2( 0.0f, 0.6f), vec2( 0.4f, 0.6f), vec2( 0.8f, 0.6f)
          , vec2(-0.8f, 0.4f), vec2(-0.4f, 0.4f), vec2( 0.0f, 0.4f), vec2( 0.4f, 0.4f), vec2( 0.8f, 0.4f)
          , vec2(-0.8f, 0.2f), vec2(-0.4f, 0.2f), vec2( 0.0f, 0.2f), vec2( 0.4f, 0.2f), vec2( 0.8f, 0.2f)
          , vec2(-0.8f, 0.0f), vec2(-0.4f, 0.0f), vec2( 0.0f, 0.0f), vec2( 0.4f, 0.0f), vec2( 0.8f, 0.0f)
          , vec2(-0.8f,-0.2f), vec2(-0.4f,-0.2f), vec2( 0.0f,-0.2f), vec2( 0.4f,-0.2f), vec2( 0.8f,-0.2f)
          , vec2(-0.8f,-0.4f), vec2(-0.4f,-0.4f), vec2( 0.0f,-0.4f), vec2( 0.4f,-0.4f), vec2( 0.8f,-0.4f)
          , vec2(-0.8f,-0.6f), vec2(-0.4f,-0.6f), vec2( 0.0f,-0.6f), vec2( 0.4f,-0.6f), vec2( 0.8f,-0.6f)
          , vec2(-0.8f,-0.8f), vec2(-0.4f,-0.8f), vec2( 0.0f,-0.8f), vec2( 0.4f,-0.8f), vec2( 0.8f,-0.8f) })
          , GL_STATIC_DRAW );

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setBuffer(m_buffer, 0, sizeof(vec2));
        m_vao->binding(0)->setFormat(2, GL_FLOAT);
        m_vao->enable(0);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shaderProgram->use();

        m_defaultPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 0, 5);

        m_thinnestPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 5, 5);

        m_thinPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 10, 5);

        m_normalPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 15, 5);

        m_thickPointSizeState->apply();

        m_vao->drawArrays(GL_POINTS, 20, 1);
        m_disableRasterizerState->apply();
        m_vao->drawArrays(GL_POINTS, 21, 1);
        m_enableRasterizerState->apply();
        m_vao->drawArrays(GL_POINTS, 22, 1);
        m_disableRasterizerState->apply();
        m_vao->drawArrays(GL_POINTS, 23, 1);
        m_enableRasterizerState->apply();
        m_vao->drawArrays(GL_POINTS, 24, 1);

        m_normalPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 25, 5);

        m_thinPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 30, 5);

        m_thinnestPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 35, 5);

        m_defaultPointSizeState->apply();
        m_vao->drawArrays(GL_POINTS, 35, 5);

        m_shaderProgram->release();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer> m_buffer;
    globjects::ref_ptr<globjects::Program> m_shaderProgram;

    globjects::ref_ptr<globjects::State> m_defaultPointSizeState;
    globjects::ref_ptr<globjects::State> m_thinnestPointSizeState;
    globjects::ref_ptr<globjects::State> m_thinPointSizeState;
    globjects::ref_ptr<globjects::State> m_normalPointSizeState;
    globjects::ref_ptr<globjects::State> m_thickPointSizeState;
    globjects::ref_ptr<globjects::State> m_disableRasterizerState;
    globjects::ref_ptr<globjects::State> m_enableRasterizerState;
};


int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "OpenGL States Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
