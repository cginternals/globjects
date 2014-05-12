

#include <glow/Buffer.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/debugmessageoutput.h>
#include <glow/StaticStringSource.h>

#include <glowwindow/Window.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowutils/StringTemplate.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

using namespace glowwindow;

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

        glow::debugmessageoutput::enable();

        gl::ClearColor(0.2f, 0.3f, 0.4f, 1.f);


        
        glowutils::StringTemplate* vertexShaderSource = new glowutils::StringTemplate(new glow::StaticStringSource(vertexShaderCode));
        glowutils::StringTemplate* fragmentShaderSource = new glowutils::StringTemplate(new glow::StaticStringSource(fragmentShaderCode));
        
        
#ifdef MAC_OS
        vertexShaderSource->replace("#version 140", "#version 150");
        fragmentShaderSource->replace("#version 140", "#version 150");
#endif
        
        
        
        cornerBuffer = new glow::Buffer();
        cornerBuffer->ref();
		program = new glow::Program();
        program->ref();
		vao = new glow::VertexArrayObject();
        vao->ref();

		program->attach(
                        new glow::Shader(gl::VERTEX_SHADER, vertexShaderSource),
                        new glow::Shader(gl::FRAGMENT_SHADER, fragmentShaderSource)
                        );

        cornerBuffer->setData(std::array<glm::vec2, 4>{ {
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1)
        } });

        vao->binding(0)->setAttribute(0);
		vao->binding(0)->setBuffer(cornerBuffer, 0, sizeof(glm::vec2));
        vao->binding(0)->setFormat(2, gl::FLOAT);
        vao->enable(0);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        gl::Viewport(0, 0, event.width(), event.height());

    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


		program->use();
        vao->drawArrays(gl::TRIANGLE_STRIP, 0, 4);
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

private:
	glow::VertexArrayObject* vao;
	glow::Buffer* cornerBuffer;
	glow::Program* program;

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
