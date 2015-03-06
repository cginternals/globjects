
#include <memory>

#include <glm/gtc/constants.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/extension.h>

#include <globjects/logging.h>
#include <globjects/globjects.h>

#include <globjects/base/File.h>

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
using namespace globjects;

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
        m_texture.reset(Texture::createDefault(GL_TEXTURE_2D));

        m_texture->image2D(0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
        m_texture->bindImageTexture(0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    }

    void createAndSetupShaders()
    {
        m_computeSource.reset(new File("data/computeshader/cstest.comp"));
        m_computeShader.reset(new Shader(GL_COMPUTE_SHADER, m_computeSource.get()));

        m_computeProgram.reset(new Program());
        m_computeProgram->attach(m_computeShader.get());

        m_textureUniform.reset(new Uniform<unsigned int>("destTex", 0));
        m_rollUniform.reset(new Uniform<float>("roll", 0.0f));

        m_computeProgram->attach(m_textureUniform.get(), m_rollUniform.get());
    }

    void createAndSetupGeometry()
    {
        m_quad.reset(new ScreenAlignedQuad(m_texture.get()));
        m_quad->setSamplerUniform(0);
    }

    virtual void initialize(Window & window) override
    {
        WindowEventHandler::initialize(window);

        if (!hasExtension(GLextension::GL_ARB_compute_shader))
        {
            critical() << "Compute shaders are not supported";

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

        m_rollUniform->set(static_cast<float>(m_frame) * 0.01f);

        m_texture->bind();

        m_computeProgram->dispatchCompute(512 / 16, 512 / 16, 1); // 512^2 threads in blocks of 16^2
        m_computeProgram->release();

        m_quad->draw();
    }
    
protected:
    std::unique_ptr<Texture> m_texture;

    std::unique_ptr<Program> m_computeProgram;
    std::unique_ptr<ScreenAlignedQuad> m_quad;
    std::unique_ptr<AbstractStringSource> m_computeSource;
    std::unique_ptr<Shader> m_computeShader;
    std::unique_ptr<Uniform<unsigned int>> m_textureUniform;
    std::unique_ptr<Uniform<float>> m_rollUniform;

    unsigned int m_frame;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"       << "Close example";
    info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"       << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"       << "Toggle vertical sync";
    info() << "\t" << "F5" << "\t\t"        << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 3);
    format.setProfile(ContextFormat::Profile::Core);
    format.setForwardCompatible(true);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "Compute Shader Example"))
        return 1;

    window.show();

    return MainLoop::run();
}
