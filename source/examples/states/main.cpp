
#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/State.h>

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

        m_defaultPointSizeState  = new State();
        m_defaultPointSizeState->pointSize(getFloat(GL_POINT_SIZE));
        m_thinnestPointSizeState = new State();
        m_thinnestPointSizeState->pointSize(2.0f);
        m_thinPointSizeState     = new State();
        m_thinPointSizeState->pointSize(5.0f);
        m_normalPointSizeState   = new State();
        m_normalPointSizeState->pointSize(10.0f);
        m_thickPointSizeState    = new State();
        m_thickPointSizeState->pointSize(20.0f);
        m_disableRasterizerState = new State();
        m_disableRasterizerState->enable(GL_RASTERIZER_DISCARD);
        m_enableRasterizerState  = new State();
        m_enableRasterizerState->disable(GL_RASTERIZER_DISCARD);

        m_vao = new VertexArray();
        m_buffer = new Buffer();

        m_shaderProgram = new Program();
        m_shaderProgram->attach(
            Shader::fromFile(GL_VERTEX_SHADER, "data/states/standard.vert")
          , Shader::fromFile(GL_FRAGMENT_SHADER, "data/states/standard.frag"));
        
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

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

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

protected:
    ref_ptr<VertexArray> m_vao;
    ref_ptr<Buffer> m_buffer;
    ref_ptr<Program> m_shaderProgram;

    ref_ptr<State> m_defaultPointSizeState;
    ref_ptr<State> m_thinnestPointSizeState;
    ref_ptr<State> m_thinPointSizeState;
    ref_ptr<State> m_normalPointSizeState;
    ref_ptr<State> m_thickPointSizeState;
    ref_ptr<State> m_disableRasterizerState;
    ref_ptr<State> m_enableRasterizerState;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"       << "Toggle vertical sync";

    ContextFormat format;
    format.setVersion(3, 0);
    format.setForwardCompatible(true);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "OpenGL States Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
