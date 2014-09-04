

#include <algorithm>
#include <random>

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

    if (window.create(format, "Simple Texture Example"))
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
    static const int w(256);
    static const int h(256);
	unsigned char data[w * h * 4];

    std::random_device rd;
    std::mt19937 generator(rd());

    std::poisson_distribution<> r(0.2);

    for (int i = 0; i < w * h * 4; ++i)
        data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));

    m_texture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);

	m_texture->image2D(0, gl::GL_RGBA8, w, h, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data);

}

void EventHandler::createAndSetupGeometry()
{
	m_quad = new ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}
