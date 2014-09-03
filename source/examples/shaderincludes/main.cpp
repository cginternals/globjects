#include <glbinding/gl/gl.h>

#include <globjects-base/File.h>

#include <globjects/NamedString.h>
#include <globjects/Shader.h>
#include <globjects/DebugMessage.h>

#include <globjects-utils/ScreenAlignedQuad.h>
#include <globjects-utils/globjects-utils.h>
#include <globjects-utils/StringTemplate.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <ExampleWindowEventHandler.h>



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

        glo::DebugMessage::enable();

        gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        glo::NamedString::create("/shaderincludes/color.glsl", new glo::File("data/shaderincludes/color.glsl"));

        gloutils::StringTemplate* fragmentShaderString = new gloutils::StringTemplate(new glo::File("data/shaderincludes/test.frag"));

#ifdef MAC_OS
        fragmentShaderString->replace("#version 140", "#version 150");
#endif

        m_quad = new gloutils::ScreenAlignedQuad(new glo::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderString));
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();
        int side = std::min<int>(width, height);

        gl::glViewport((width - side) / 2, (height - side) / 2, side, side);
    }

    virtual void paintEvent(PaintEvent &) override
    {
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_quad->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyReleaseEvent(KeyEvent & event) override
    {
        if (GLFW_KEY_F5 == event.key())
            glo::File::reloadAll();
    }

protected:
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quad;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glo::info() << "Usage:";
    glo::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glo::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Shading Language Include Example"))
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
