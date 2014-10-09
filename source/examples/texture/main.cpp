
#include <algorithm>
#include <random>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>
#include <globjects/Texture.h>

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

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

        createAndSetupTexture();
	    createAndSetupGeometry();
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_quad->draw();
    }

    void createAndSetupTexture()
    {
        static const int w(256);
        static const int h(256);

        unsigned char data[w * h * 4];

        std::random_device rd;
        std::mt19937 generator(rd());

        std::poisson_distribution<> r(0.2);

        for (int i = 0; i < w * h * 4; ++i)
            data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));

        m_texture = Texture::createDefault(GL_TEXTURE_2D);
        m_texture->image2D(0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void createAndSetupGeometry()
    {
        m_quad = new ScreenAlignedQuad(m_texture);
        m_quad->setSamplerUniform(0);
    }

protected:
    ref_ptr<Texture> m_texture;
    ref_ptr<ScreenAlignedQuad> m_quad;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"       << "Toggle vertical sync";

    ContextFormat format;
    format.setVersion(3, 0);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Texture Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
