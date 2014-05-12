

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/Texture.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include "RawFile.h"

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

    void createAndSetupTexture();
	void createAndSetupGeometry();

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glow::debugmessageoutput::enable();

        gl::ClearColor(0.2f, 0.3f, 0.4f, 1.f);


        createAndSetupTexture();
	    createAndSetupGeometry();
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

protected:
    glow::ref_ptr<glow::Texture> m_texture;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
};


/** This example shows ... .
*/
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

    if (window.create(format, "glraw Texture Example"))
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

void EventHandler::createAndSetupTexture()
{
    RawFile raw("data/glraw-texture/dog-on-pillow.raw");
    if (!raw.isValid())
        return;

    m_texture = glow::Texture::createDefault(gl::TEXTURE_2D);

    m_texture->compressedImage2D(0, gl::COMPRESSED_RGBA_S3TC_DXT1_EXT, glm::ivec2(256, 256), 0, static_cast<gl::GLsizei>(raw.size()), raw.data());
}

void EventHandler::createAndSetupGeometry()
{
	m_quad = new glowutils::ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}
