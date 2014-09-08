#include <glbinding/gl/gl.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/DebugMessage.h>
#include <globjects/base/StaticStringSource.h>

#include <common/StringTemplate.h>
#include <common/Window.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>

namespace {
    const char* vertexShaderCode = R"(
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

class EventHandler : public ExampleWindowEventHandler
{
public:
	EventHandler()
    : vao(nullptr)
    , cornerBuffer(nullptr)
    , program(nullptr)
    {
    }

    virtual ~EventHandler()
    {
        vao->destroy();
        cornerBuffer->destroy();
        program->destroy();
    }

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glo::DebugMessage::enable();

        gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);


        
        StringTemplate* vertexShaderSource = new StringTemplate(new glo::StaticStringSource(vertexShaderCode));
        StringTemplate* fragmentShaderSource = new StringTemplate(new glo::StaticStringSource(fragmentShaderCode));
        
        
#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        
        
        cornerBuffer = new glo::Buffer();
        cornerBuffer->ref();
		program = new glo::Program();
        program->ref();
		vao = new glo::VertexArray();
        vao->ref();

		program->attach(
            new glo::Shader(gl::GL_VERTEX_SHADER, vertexShaderSource),
            new glo::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource)
        );

        cornerBuffer->setData(std::array<glm::vec2, 4>{ {
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1)
        } }, gl::GL_STATIC_DRAW);

        vao->binding(0)->setAttribute(0);
		vao->binding(0)->setBuffer(cornerBuffer, 0, sizeof(glm::vec2));
        vao->binding(0)->setFormat(2, gl::GL_FLOAT);
        vao->enable(0);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        gl::glViewport(0, 0, event.width(), event.height());

    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);


		program->use();
        vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

private:
	glo::VertexArray* vao;
	glo::Buffer* cornerBuffer;
	glo::Program* program;

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
