
#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <globjects/Texture.h>
#include <globjects/DebugMessage.h>

#include <common/ScreenAlignedQuad.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include "RawFile.h"

#include <ExampleWindowEventHandler.h>


using namespace gl;
using namespace glm;

class EventHandler : public ExampleWindowEventHandler
{
public:
    EventHandler()
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture()
    {
        RawFile raw("data/glraw-texture/dog-on-pillow.raw");
        if (!raw.isValid())
            return;

        m_texture = globjects::Texture::createDefault(GL_TEXTURE_2D);

        m_texture->compressedImage2D(0, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 
            ivec2(256, 256), 0, static_cast<GLsizei>(raw.size()), raw.data());
    }

    void createAndSetupGeometry()
    {
        m_quad = new ScreenAlignedQuad(m_texture);
        m_quad->setSamplerUniform(0);
    }

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        globjects::DebugMessage::enable();

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        createAndSetupTexture();
	    createAndSetupGeometry();
    }
    
    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        const int width  = event.width();
        const int height = event.height();
        const int side = std::min<int>(width, height);

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

protected:
    globjects::ref_ptr<globjects::Texture> m_texture;
    globjects::ref_ptr<ScreenAlignedQuad> m_quad;
};


int main(int /*argc*/, char* /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "glraw Texture Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
