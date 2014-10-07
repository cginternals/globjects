
#include <glm/gtx/constants.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>

#include <globjects/logging.h>
#include <globjects/globjects.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Texture.h>

#include <common/ScreenAlignedQuad.h>
#include <common/Context.h>
#include <common/ContextFormat.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>


using namespace gl;
using namespace glm;

class EventHandler : public WindowEventHandler
{
public:
    EventHandler()
    :   m_frame(0)
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture()
    {
        m_texture = globjects::Texture::createDefault(GL_TEXTURE_2D);

        m_texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
        m_texture->bindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    }

    void createAndSetupShaders()
    {
        m_computeProgram = new globjects::Program();
        m_computeProgram->attach(globjects::Shader::fromFile(GL_COMPUTE_SHADER, "data/computeshader/cstest.comp"));

        m_computeProgram->setUniform("destTex", 0);
    }

    void createAndSetupGeometry()
    {
        m_quad = new ScreenAlignedQuad(m_texture);
        m_quad->setSamplerUniform(0);
    }

    virtual void initialize(Window & window) override
    {
        WindowEventHandler::initialize(window);

        if (!globjects::hasExtension(GLextension::GL_ARB_compute_shader))
        {
            globjects::critical() << "Compute shaders are not supported";

            window.close();
            return;
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.f);

	    createAndSetupTexture();
	    createAndSetupShaders();
	    createAndSetupGeometry();
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ++m_frame %= static_cast<int>(200 * pi<double>());

	    m_computeProgram->setUniform("roll", static_cast<float>(m_frame) * 0.01f);

	    m_texture->bind();

        m_computeProgram->dispatchCompute(512 / 16, 512 / 16, 1); // 512^2 threads in blocks of 16^2
	    m_computeProgram->release();

        m_quad->draw();
    }
    
protected:
    globjects::ref_ptr<globjects::Texture> m_texture;

    globjects::ref_ptr<globjects::Program> m_computeProgram;
    globjects::ref_ptr<ScreenAlignedQuad> m_quad;

    unsigned int m_frame;
};


int main(int /*argc*/, char * /*argv*/[])
{
    globjects::info() << "Usage:";
    globjects::info() << "\t" << "ESC" << "\t\t"       << "Close example";
    globjects::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    globjects::info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    globjects::info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 3);
    format.setProfile(ContextFormat::CoreProfile);

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Compute Shader Example"))
        return 1;

    window.context()->setSwapInterval(Context::NoVerticalSyncronization);
    window.show();

    return MainLoop::run();
}
