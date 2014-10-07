
#include <glbinding/gl/gl.h>

#include <globjects/DebugMessage.h>
#include <globjects/NamedString.h>
#include <globjects/Shader.h>

#include <globjects/base/File.h>

#include <common/ScreenAlignedQuad.h>
#include <common/StringTemplate.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;

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

        globjects::DebugMessage::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        globjects::NamedString::create("/color.glsl", new globjects::File("data/shaderincludes/color.glsl"));

        m_quad = new ScreenAlignedQuad(globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/shaderincludes/test.frag"));
    }
    
    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_quad->draw();
    }

protected:
    globjects::ref_ptr<ScreenAlignedQuad> m_quad;
};


int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    globjects::info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Shading Language Include Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
