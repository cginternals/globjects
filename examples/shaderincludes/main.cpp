
#include <GL/glew.h>

#include <glow/global.h>
#include <glow/Shader.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/File.h>
#include <glowutils/File.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/global.h>
#include <glowutils/StringTemplate.h>

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

    virtual void initialize(Window & ) override
    {
        glow::debugmessageoutput::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);
        CheckGLError();

        glow::createNamedString("/shaderincludes/color.glsl", new glowutils::File("data/shaderincludes/color.glsl"));

      glowutils::StringTemplate* fragmentShaderString = new glowutils::StringTemplate(new glowutils::File("data/shaderincludes/test.frag"));
#ifdef MAC_OS
      fragmentShaderString->replace("#version 140", "#version 150");
#endif

      m_quad = new glowutils::ScreenAlignedQuad(new glow::Shader(GL_FRAGMENT_SHADER, fragmentShaderString));
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        int width = event.width();
        int height = event.height();
        int side = std::min<int>(width, height);

        glViewport((width - side) / 2, (height - side) / 2, side, side);
        CheckGLError();
    }

    virtual void paintEvent(PaintEvent &) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CheckGLError();

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
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
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
