
#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>

#include <common/ScreenAlignedQuad.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
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

        if (!hasExtension(GLextension::GL_ARB_shader_storage_buffer_object))
        {
            critical() << "Shader storage buffer objects not supported.";

            window.close();
            return;
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);


        m_quad = new ScreenAlignedQuad(Shader::fromFile(GL_FRAGMENT_SHADER, "data/ssbo/ssbo.frag"));

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

        m_buffer = new Buffer();
        m_buffer->setData(sizeof(data), data, GL_STATIC_DRAW);

        m_buffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_quad->draw();
    }

protected:
    ref_ptr<ScreenAlignedQuad> m_quad;
    ref_ptr<Buffer> m_buffer;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

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
