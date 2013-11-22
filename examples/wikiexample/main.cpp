#include <GL/glew.h>

#include <glow/Buffer.h>
#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>

#include <glowwindow/Window.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glowwindow;

namespace {
    const char* vertexShaderCode = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

in vec2 corner;

out vec4 color;

void main()
{
    gl_Position = vec4(corner * 2.0 - 1.0, 0.0, 1.0);
    color = vec4(corner, 0.0, 1.0);
}

)";
    const char* fragmentShaderCode = R"(
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
    {
    }

    virtual ~EventHandler()
    {
    }

    virtual void initialize(Window & window) override
    {
        glow::DebugMessageOutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);
    }
    
    virtual void resizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glow::VertexArrayObject vao;
        glow::Buffer cornerBuffer(GL_ARRAY_BUFFER);
        glow::Program program;

        program.attach(
            glow::Shader::fromString(GL_VERTEX_SHADER, vertexShaderCode),
            glow::Shader::fromString(GL_FRAGMENT_SHADER, fragmentShaderCode)
        );

        cornerBuffer.setData(glow::Array<glm::vec2>({
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 1)
        }));

        vao.binding(0)->setAttribute(program.getAttributeLocation("corner"));
        vao.binding(0)->setBuffer(&cornerBuffer, 0, sizeof(glm::vec2));
        vao.binding(0)->setFormat(2, GL_FLOAT);
        vao.enable(program.getAttributeLocation("corner"));

        program.use();
        vao.drawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

};

int main(int argc, char* argv[])
{
    ContextFormat format;
    format.setVersion(3, 0);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Wiki Example"))
    {
        window.context()->setSwapInterval(Context::VerticalSyncronization);

        window.show();

        return MainLoop::run();
    }
    else
    {
        return 1;
    }
}
