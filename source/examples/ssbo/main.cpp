
#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>

#include <globjects/Buffer.h>
#include <globjects/DebugMessage.h>
#include <globjects/Program.h>

#include <globjects/base/File.h>

#include <common/ScreenAlignedQuad.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>


using namespace gl;

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

        if (!globjects::hasExtension(GLextension::GL_ARB_shader_storage_buffer_object))
        {
            globjects::critical() << "Shader storage buffer objects not supported.";

            window.close();
            return;
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);


        m_quad = new ScreenAlignedQuad(globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/ssbo/ssbo.frag"));

        m_quad->program()->setUniform("maximum",     10);
        m_quad->program()->setUniform("rowCount",    10);
        m_quad->program()->setUniform("columnCount", 10);

        int data[] = {
            1,2,3,4,5,6,7,8,9,10,
            10,1,2,3,4,5,6,7,8,9,
            9,10,1,2,3,4,5,6,7,8,
            8,9,10,1,2,3,4,5,6,7,
            7,8,9,10,1,2,3,4,5,6,
            6,7,8,9,10,1,2,3,4,5,
            5,6,7,8,9,10,1,2,3,4,
            4,5,6,7,8,9,10,1,2,3,
            3,4,5,6,7,8,9,10,1,2,
            2,3,4,5,6,7,8,9,10,1 };

        m_buffer = new globjects::Buffer();
        m_buffer->setData(sizeof(data), data, GL_STATIC_DRAW);

        m_buffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        const int width  = event.width();
        const int height = event.height();
        const int side   = std::min<int>(width, height);

        glViewport((width - side) / 2, (height - side) / 2, side, side);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_quad->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyReleaseEvent(KeyEvent & event) override
    {
        if (GLFW_KEY_F5 == event.key())
            globjects::File::reloadAll();
    }

protected:
    globjects::ref_ptr<ScreenAlignedQuad> m_quad;
    globjects::ref_ptr<globjects::Buffer> m_buffer;
};


int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    globjects::info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 3);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Shader Storage Buffer Objects Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
