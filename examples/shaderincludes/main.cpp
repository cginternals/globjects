
#include <GL/glew.h>

#include <glow/NamedStrings.h>
#include <glow/DebugMessageOutput.h>

#include <glowutils/File.h>
#include <glowutils/FileRegistry.h>
#include <glowutils/ScreenAlignedQuad.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>

using namespace glowwindow;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupShaders();

    virtual void initialize(Window & window) override
    {
        glow::DebugMessageOutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        glow::NamedStrings::createNamedString("/shaderincludestest/colorReset", "color = vec4(0.4, 0.5, 0.6, 1.0);");

        m_quad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/shaderincludes/test.frag"));
    }
    
    virtual void resizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();
        int side = std::min<int>(width, height);
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
            glowutils::FileRegistry::instance().reloadAll();
    }

protected:
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
};


/** This example shows ... .
*/
int main(int argc, char* argv[])
{
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
