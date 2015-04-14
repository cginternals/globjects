
#include <memory>

#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include <globjects/base/File.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/State.h>
#include <globjects/Capability.h>

#include <common/Context.h>
#include <common/ContextFormat.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace glm;
using namespace globjects;

class EventHandler : public WindowEventHandler
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
        WindowEventHandler::initialize(window);

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        m_defaultPointSizeState.reset(new State());
        m_defaultPointSizeState->pointSize(getFloat(GL_POINT_SIZE));
        m_thinnestPointSizeState.reset(new State());
        m_thinnestPointSizeState->pointSize(2.0f);
        m_thinPointSizeState.reset(new State());
        m_thinPointSizeState->pointSize(5.0f);
        m_normalPointSizeState.reset(new State());
        m_normalPointSizeState->pointSize(10.0f);
        m_thickPointSizeState.reset(new State());
        m_thickPointSizeState->pointSize(20.0f);
        m_disableRasterizerState.reset(new State());
        m_disableRasterizerState->enable(GL_RASTERIZER_DISCARD);
        m_enableRasterizerState.reset(new State());
        m_enableRasterizerState->disable(GL_RASTERIZER_DISCARD);

        m_vao.reset(new VertexArray());
        m_buffer.reset(new Buffer());

        m_vertexSource.reset(new File("data/states/standard.vert"));
        m_vertexShader.reset(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()));

        m_fragmentSource.reset(new File("data/states/standard.frag"));
        m_fragmentShader.reset(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()));

        m_program.reset(new Program());
        m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
        
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
        m_vao->binding(0)->setBuffer(m_buffer.get(), 0, sizeof(vec2));
        m_vao->binding(0)->setFormat(2, GL_FLOAT);
        m_vao->enable(0);
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->use();

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

        m_program->release();
    }

protected:
    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<Buffer> m_buffer;
    std::unique_ptr<Program> m_program;
    std::unique_ptr<AbstractStringSource> m_vertexSource;
    std::unique_ptr<AbstractStringSource> m_fragmentSource;
    std::unique_ptr<Shader> m_vertexShader;
    std::unique_ptr<Shader> m_fragmentShader;

    std::unique_ptr<State> m_defaultPointSizeState;
    std::unique_ptr<State> m_thinnestPointSizeState;
    std::unique_ptr<State> m_thinPointSizeState;
    std::unique_ptr<State> m_normalPointSizeState;
    std::unique_ptr<State> m_thickPointSizeState;
    std::unique_ptr<State> m_disableRasterizerState;
    std::unique_ptr<State> m_enableRasterizerState;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"       << "Toggle vertical sync";

    ContextFormat format;
    format.setVersion(3, 1);
    format.setForwardCompatible(true);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "OpenGL States Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
