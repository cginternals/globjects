
#include <glbinding/gl/gl.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/StaticStringSource.h>

#include <common/Window.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace glm;
using namespace globjects;

namespace 
{
    const char * vertexShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 corner;

out vec4 color;

void main()
{
    gl_Position = vec4(corner * 2.0 - 1.0, 0.0, 1.0);
    color = vec4(corner, 0.0, 1.0);
}

)";
    const char * fragmentShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 fragColor;

in vec4 color;

void main()
{
    fragColor = color;
}

)";
}

class EventHandler : public WindowEventHandler
{
public:

    EventHandler()
    : m_vao(nullptr)
    , m_cornerBuffer(nullptr)
    , m_program(nullptr)
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        WindowEventHandler::initialize(window);

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        m_cornerBuffer.reset(new Buffer());
        m_program.reset(new Program());
        m_vao.reset(new VertexArray());

        m_vertexSource.reset(new StaticStringSource(vertexShaderCode));
        m_vertexShader.reset(new Shader(GL_VERTEX_SHADER, m_vertexSource.get()));

        m_fragmentSource.reset(new StaticStringSource(fragmentShaderCode));
        m_fragmentShader.reset(new Shader(GL_FRAGMENT_SHADER, m_fragmentSource.get()));

        m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

        m_cornerBuffer->setData(std::array<vec2, 4>{ {
            vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1) } }, GL_STATIC_DRAW);

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setBuffer(m_cornerBuffer.get(), 0, sizeof(vec2));
        m_vao->binding(0)->setFormat(2, GL_FLOAT);
        m_vao->enable(0);
    }
    
    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->use();
        m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

private:
    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<Buffer> m_cornerBuffer;
    std::unique_ptr<Program> m_program;
    std::unique_ptr<AbstractStringSource> m_vertexSource;
    std::unique_ptr<AbstractStringSource> m_fragmentSource;
    std::unique_ptr<Shader> m_vertexShader;
    std::unique_ptr<Shader> m_fragmentShader;
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

    if (!window.create(format, "Wiki Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
