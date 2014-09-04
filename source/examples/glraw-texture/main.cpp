
#include <glbinding/gl/gl.h>

#include <globjects/logging.h>
#include <globjects/Texture.h>
#include <globjects/DebugMessage.h>

#include <common/ScreenAlignedQuad.h>
#include <globjects-window/ContextFormat.h>
#include <globjects-window/Context.h>
#include <globjects-window/Window.h>
#include <globjects-window/WindowEventHandler.h>
#include <globjects-window/events.h>

#include "RawFile.h"

#include <ExampleWindowEventHandler.h>

using namespace glowindow;

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

        glo::DebugMessage::enable();

        gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);


        createAndSetupTexture();
	    createAndSetupGeometry();
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

protected:
    glo::ref_ptr<glo::Texture> m_texture;
    glo::ref_ptr<ScreenAlignedQuad> m_quad;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glo::info() << "Usage:";
    glo::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glo::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";

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

    m_texture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);

    m_texture->compressedImage2D(0, gl::GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, glm::ivec2(256, 256), 0, static_cast<gl::GLsizei>(raw.size()), raw.data());
}

void EventHandler::createAndSetupGeometry()
{
	m_quad = new ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}
