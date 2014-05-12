


#include <glow/Buffer.h>
#include <glow/debugmessageoutput.h>
#include <glow/Program.h>

#include <glowutils/File.h>
#include <glowutils/File.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/glowutils.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

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


        m_quad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, "data/ssbo/ssbo.frag"));

        m_quad->program()->setUniform("maximum", 10);
        m_quad->program()->setUniform("rowCount", 10);
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
            2,3,4,5,6,7,8,9,10,1
        };

        m_buffer = new glow::Buffer();
        m_buffer->setData(sizeof(data), data, gl::STATIC_DRAW);

        m_buffer->bindBase(gl::SHADER_STORAGE_BUFFER, 1);
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();
        int side = std::min<int>(width, height);

        gl::Viewport((width - side) / 2, (height - side) / 2, side, side);

    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


        m_quad->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyReleaseEvent(KeyEvent & event) override
    {
        if (GLFW_KEY_F5 == event.key())
            glowutils::File::reloadAll();
    }

protected:
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glow::ref_ptr<glow::Buffer> m_buffer;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glow::info() << "Usage:";
    glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glow::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 3);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Shader Storage Buffer Objects Example"))
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
