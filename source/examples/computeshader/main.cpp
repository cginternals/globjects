


#include <algorithm>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/constants.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Extension.h>
#include <globjects/VertexArrayObject.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>
#include <globjects/Texture.h>
#include <globjects/globjects.h>

#include <globjects-base/File.h>
#include <globjects-base/File.h>
#include <globjects-utils/ScreenAlignedQuad.h>
#include <globjects-utils/globjects-utils.h>

#include <globjects-window/Context.h>
#include <globjects-window/ContextFormat.h>
#include <globjects-window/Window.h>
#include <globjects-window/WindowEventHandler.h>
#include <globjects-window/events.h>

#include <ExampleWindowEventHandler.h>


using namespace glowindow;

class EventHandler : public ExampleWindowEventHandler
{
public:
    EventHandler()
    :   m_frame(0)
    {
    }

    virtual ~EventHandler()
    {
    }

    void createAndSetupTexture();
    void createAndSetupShaders();
	void createAndSetupGeometry();

    virtual void initialize(Window & window) override
    {
        ExampleWindowEventHandler::initialize(window);

        glo::DebugMessage::enable();

        if (!glo::hasExtension(glo::Extension::GL_ARB_compute_shader))
        {
            glo::critical() << "Compute shaders are not supported";

            window.close();

            return;
        }

        gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);


	    createAndSetupTexture();
	    createAndSetupShaders();
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


        ++m_frame %= static_cast<int>(200 * glm::pi<double>());

	    m_computeProgram->setUniform("roll", static_cast<float>(m_frame) * 0.01f);

	    m_texture->bind();

        m_computeProgram->dispatchCompute(512/16, 512/16, 1); // 512^2 threads in blocks of 16^2
	    m_computeProgram->release();

        m_quad->draw();
    }

    virtual void idle(Window & window) override
    {
        window.repaint();
    }

    virtual void keyReleaseEvent(KeyEvent & event) override
    {
        if (GLFW_KEY_F5 == event.key())
            glo::File::reloadAll();
    }

protected:
    glo::ref_ptr<glo::Texture> m_texture;

    glo::ref_ptr<glo::Program> m_computeProgram;
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quad;

    unsigned int m_frame;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
    glo::info() << "Usage:";
    glo::info() << "\t" << "ESC" << "\t\t" << "Close example";
    glo::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
    glo::info() << "\t" << "F5" << "\t\t" << "Reload shaders";

    ContextFormat format;
    format.setVersion(4, 3);
    format.setProfile(ContextFormat::CoreProfile);

    Window window;

    window.setEventHandler(new EventHandler());

    if (window.create(format, "Compute Shader Example"))
    {
        window.context()->setSwapInterval(Context::NoVerticalSyncronization);

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
    m_texture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);

    m_texture->image2D(0, gl::GL_R32F, 512, 512, 0, gl::GL_RED, gl::GL_FLOAT, nullptr);
    m_texture->bindImageTexture(0, 0, gl::GL_FALSE, 0, gl::GL_WRITE_ONLY, gl::GL_R32F);
}

void EventHandler::createAndSetupShaders()
{
    m_computeProgram = new glo::Program();
    m_computeProgram->attach(glo::Shader::fromFile(gl::GL_COMPUTE_SHADER, "data/computeshader/cstest.comp"));

    m_computeProgram->setUniform("destTex", 0);
}

void EventHandler::createAndSetupGeometry()
{
    m_quad = new gloutils::ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}
