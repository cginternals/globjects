
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

        m_texture = globjects::Texture::createDefault(GL_TEXTURE_2D);
        m_texture->image2D(0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void createAndSetupGeometry()
    {
        m_quad = new ScreenAlignedQuad(m_texture);
        m_quad->setSamplerUniform(0);
    }

protected:
    globjects::ref_ptr<globjects::Texture> m_texture;
    globjects::ref_ptr<ScreenAlignedQuad> m_quad;
};


int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";

    ContextFormat format;
    format.setVersion(3, 0);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Texture Example"))
        return 1;

    window.context()->setSwapInterval(Context::VerticalSyncronization);
    window.show();

    return MainLoop::run();
}
